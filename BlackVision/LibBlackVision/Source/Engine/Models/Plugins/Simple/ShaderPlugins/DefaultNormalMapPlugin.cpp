#include "stdafx.h"

#include "DefaultNormalMapPlugin.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


#include "Assets/DefaultAssets.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string        DefaultNormalMapPlugin::PARAM::NORMAL_MAP_MAT  = "normalMapMat";

// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultNormalMapPluginDesc::DefaultNormalMapPluginDesc                          ()
    : BasePluginDescriptor( UID(), "normal map", "norm" )
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
    DefaultParamValModelPtr vsModel = std::make_shared< DefaultParamValModel >();

    SimpleTransformEvaluatorPtr trTxEvaluator = ParamValEvaluatorFactory::CreateSimpleTransformEvaluator( DefaultNormalMapPlugin::PARAM::NORMAL_MAP_MAT, timeEvaluator );
    
    helper.SetOrCreatePluginModel();
    helper.SetOrCreatePSModel();
    
    vsModel->RegisterAll( trTxEvaluator );
    model->SetVertexShaderChannelModel( vsModel );

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
bool DefaultNormalMapPlugin::SetPrevPlugin( IPluginPtr prev )
{
    if( BasePlugin::SetPrevPlugin( prev ) )
    {
        HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
        auto ctx = m_psc->GetRendererContext();
        ctx->cullCtx->enabled = false;
        return true;
    }
    else
        return false;

}

// *************************************
// 
DefaultNormalMapPlugin::DefaultNormalMapPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

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
    return GetPrevPlugin()->IsValid();
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

        // If texture doesn't exists, read fallback texture. 
        if( txDesc == nullptr )
        {
            txAssetDescr = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();
            txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultNormalMapPluginDesc::TextureName() );
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
IVertexShaderChannelConstPtr        DefaultNormalMapPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                DefaultNormalMapPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, PARAM::NORMAL_MAP_MAT.c_str() );
    
    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    m_psc->PostUpdate();    
}

} // model
} // bv
