#include "stdafx.h"

#include "DefaultBlendTexturePlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IValue.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/DefaultAssets.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


typedef ParamEnum< DefaultBlendTexturePlugin::BlendingMode > ParamEnumBlendingMode;

const std::string        DefaultBlendTexturePlugin::PARAMS::ALPHA             = "alpha2";
const std::string        DefaultBlendTexturePlugin::PARAMS::BLENDING_MODE     = "blendingMode";


VoidPtr    ParamEnumBlendingMode::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template<>
static IParameterPtr        ParametersFactory::CreateTypedParameter< DefaultBlendTexturePlugin::BlendingMode >                 ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultBlendTexturePlugin::BlendingMode >( name, timeline );
}


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultBlendTexturePluginDesc::DefaultBlendTexturePluginDesc                          ()
    : BasePluginDescriptor( UID(), "blend texture", "blend" )
{
}

// *******************************
//
IPluginPtr              DefaultBlendTexturePluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultBlendTexturePlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultBlendTexturePluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txBlendMat", timeEvaluator );
    
    helper.SetOrCreatePluginModel();

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultBlendTexturePlugin::PARAMS::ALPHA, 1.f, true );
    helper.AddSimpleParam( DefaultBlendTexturePlugin::PARAMS::BLENDING_MODE, (int)DefaultBlendTexturePlugin::BlendingMode::BM_Normal, true );

    // In future it would be better to have enum parameter
    //helper.AddParam< IntInterpolator, DefaultBlendTexturePlugin::BlendingMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumBlendingMode >
    //    ( DefaultBlendTexturePlugin::PARAMS::BLENDING_MODE, DefaultBlendTexturePlugin::BlendingMode::BM_Normal, true, false );

    //Register all parameters and evaloators in models
    vsModel->RegisterAll( trTxEvaluator );

    //Set models structure
    model->SetVertexShaderChannelModel( vsModel );

    //Set default values of all parameters
    trTxEvaluator->Parameter()->Transform().InitializeDefaultSRT();
    trTxEvaluator->Parameter()->Transform().SetCenter( glm::vec3( 0.5, 0.5, 0.0 ), 0.0f );

    return model;
}

// *******************************
//
std::string             DefaultBlendTexturePluginDesc::UID                       ()
{
    return "DEFAULT_BLEND_TEXTURE";
}

// *******************************
// 
std::string             DefaultBlendTexturePluginDesc::TextureName               ()
{
    return "BlendTex0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
bool DefaultBlendTexturePlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
        return true;
    }
    else
        return false;


}

// *************************************
// 
DefaultBlendTexturePlugin::DefaultBlendTexturePlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_psc( nullptr )
    , m_vsc( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );
}

// *************************************
// 
DefaultBlendTexturePlugin::~DefaultBlendTexturePlugin         ()
{}

// *************************************
// 
bool							DefaultBlendTexturePlugin::IsValid     () const
{
    return ( GetPrevPlugin()->IsValid() );
}

// *************************************
// 
bool                            DefaultBlendTexturePlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );
    
    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultBlendTexturePluginDesc::TextureName() );

        // If texture doesn't exists, read fallback texture. 
        if( txDesc == nullptr )
        {
            txAssetDescr = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();
            txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultBlendTexturePluginDesc::TextureName() );
        }

        if( txDesc != nullptr )
        {
            auto txData = m_psc->GetTexturesDataImpl();
            auto replacedTex = txData->GetTexture( 0 );

            SamplerStateModelPtr newSamplerStateModel = replacedTex != nullptr ? replacedTex->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

            txDesc->SetSamplerState( newSamplerStateModel );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
            
            txData->SetTexture( 0, txDesc );
            SetAsset( 0, LAsset( txDesc->GetName(), assetDescr, txDesc->GetSamplerState() ) );

            HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

            m_textureWidth = txAssetDescr->GetOrigTextureDesc()->GetWidth();
            m_textureHeight = txAssetDescr->GetOrigTextureDesc()->GetHeight();


            return true;
        }

    }
    return false;
}


// *************************************
// 
IPixelShaderChannelPtr              DefaultBlendTexturePlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
IVertexShaderChannelConstPtr        DefaultBlendTexturePlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultBlendTexturePlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, "txBlendMat" );    
    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();    
}


// *************************************
// 
SizeType									DefaultBlendTexturePlugin::GetTextureWidth             () const
{
    return m_textureWidth;
}

// *************************************
//
SizeType									DefaultBlendTexturePlugin::GetTextureHeight            () const
{
    return m_textureHeight;
}


} // model
} // bv
