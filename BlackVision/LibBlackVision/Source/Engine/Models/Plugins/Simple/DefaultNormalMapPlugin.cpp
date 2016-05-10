#include "stdafx.h"

#include "DefaultNormalMapPlugin.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine\Interfaces\IValue.h"

#include "Engine/Models/Plugins/Channels/Geometry/VacAABB.h"

#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Engine/Models/Plugins/HelperUVGenerator.h"

#include "Assets/DefaultAssets.h"


namespace bv { namespace model {


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultNormalMapPluginDesc::DefaultNormalMapPluginDesc                          ()
    : BasePluginDescriptor( UID(), "normalmap", "norm" )
{
}

// *******************************
//
IPluginPtr              DefaultNormalMapPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultNormalMapPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultNormalMapPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();
    DefaultParamValModelPtr vsModel      = std::make_shared< DefaultParamValModel >();

    //Create all parameters and evaluators
    SimpleTransformEvaluatorPtr trTxEvaluator    = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( "txMat", timeEvaluator );
    
    helper.CreatePluginModel();

    helper.CreatePSModel();

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
std::string             DefaultNormalMapPluginDesc::UID                       ()
{
    return "DEFAULT_NORMAL_MAP";
}

// *******************************
// 
std::string             DefaultNormalMapPluginDesc::TextureName               ()
{
    return "NormMap0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
void DefaultNormalMapPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
    
    ctx->alphaCtx->blendEnabled = true;
    ctx->alphaCtx->srcRGBBlendMode = model::AlphaContext::SrcBlendMode::SBM_SRC_ALPHA;
    ctx->alphaCtx->dstRGBBlendMode = model::AlphaContext::DstBlendMode::DBM_ONE_MINUS_SRC_ALPHA;
}

// *************************************
// 
DefaultNormalMapPlugin::DefaultNormalMapPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
    , m_psc( nullptr )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

    SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );
}

// *************************************
// 
DefaultNormalMapPlugin::~DefaultNormalMapPlugin         ()
{
}

// *************************************
// 
bool							DefaultNormalMapPlugin::IsValid     () const
{
    return m_prevPlugin->IsValid();
}

// *************************************
// 
bool                            DefaultNormalMapPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );
    
    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultNormalMapPluginDesc::TextureName() );

        if( txDesc != nullptr )
        {
            txDesc->SetSamplerState( SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() ) );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
            
            auto txData = m_psc->GetTexturesDataImpl();
            txData->SetTexture( 0, txDesc );
            SetAsset( 0, LAsset( txDesc->GetName(), assetDescr, txDesc->GetSamplerState() ) );

            HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

            return true;
        }

    }
    return false;
}

// *************************************
// 
IPixelShaderChannelPtr              DefaultNormalMapPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
void                                DefaultNormalMapPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, "txMat" );
    
    HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_psc->PostUpdate();    
}

} // model
} // bv
