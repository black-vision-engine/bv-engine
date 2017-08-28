#include "stdafx.h"

#include "DefaultParallaxMapPlugin.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


#include "Assets/DefaultAssets.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string        DefaultParallaxMapPlugin::PARAM::HEIGHT_SCALE      = "heightScale";
const std::string        DefaultParallaxMapPlugin::PARAM::MIN_SAMPLES_NUM   = "minSamplesNum";
const std::string        DefaultParallaxMapPlugin::PARAM::MAX_SAMPLES_NUM   = "maxSamplesNum";


// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultParallaxMapPluginDesc::DefaultParallaxMapPluginDesc                          ()
    : BasePluginDescriptor( UID(), "parallax map", "prlx" )
{
}

// *******************************
//
IPluginPtr              DefaultParallaxMapPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultParallaxMapPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultParallaxMapPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model  = helper.GetModel();

    helper.SetOrCreatePluginModel();
    
    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( DefaultParallaxMapPlugin::PARAM::HEIGHT_SCALE, 0.1f, true );
    helper.AddSimpleParam( DefaultParallaxMapPlugin::PARAM::MIN_SAMPLES_NUM, 10, true );
    helper.AddSimpleParam( DefaultParallaxMapPlugin::PARAM::MAX_SAMPLES_NUM, 20, true );

    return model;
}

// *******************************
//
std::string             DefaultParallaxMapPluginDesc::UID                       ()
{
    return "DEFAULT_PARALLAX_MAP";
}

// *******************************
// 
std::string             DefaultParallaxMapPluginDesc::TextureName               ()
{
    return "ParallaxMap0";
}

//FIXME: dodawanie kanalow w ten sposob (przez przypisanie na m_<xxx>channel powoduje bledy, trzeba to jakos poprawic, zeby bylo wiadomo, o co chodzi
//FIXME: teraz zle dodanie wychodzi dopiero po odpaleniu silnika, a to jest oczywisty blad

// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
bool DefaultParallaxMapPlugin::SetPrevPlugin( IPluginPtr prev )
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
DefaultParallaxMapPlugin::DefaultParallaxMapPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
{
    m_psc = DefaultPixelShaderChannel::Create( model->GetPixelShaderChannelModel() );

    SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< TextureAssetDesc >() );
}

// *************************************
// 
DefaultParallaxMapPlugin::~DefaultParallaxMapPlugin         ()
{
}

// *************************************
// 
bool							DefaultParallaxMapPlugin::IsValid     () const
{
    return GetPrevPlugin()->IsValid();
}

// *************************************
// 
bool                            DefaultParallaxMapPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );
    
    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        bool success = true;

        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultParallaxMapPluginDesc::TextureName() );

        // If texture doesn't exists, read fallback texture. 
        if( txDesc == nullptr )
        {
            txAssetDescr = DefaultAssets::Instance().GetFallbackDesc< TextureAssetDesc >();
            txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultParallaxMapPluginDesc::TextureName() );

            success = false;
        }

        if( txDesc != nullptr )
        {
            auto txData = m_psc->GetTexturesDataImpl();
            auto replacedTex = txData->GetTexture( 0 );

            SamplerStateModelPtr newSamplerStateModel = replacedTex != nullptr ? replacedTex->GetSamplerState() : SamplerStateModel::Create( m_pluginParamValModel->GetTimeEvaluator() );

            txDesc->SetSamplerState( newSamplerStateModel );
            txDesc->SetSemantic( DataBuffer::Semantic::S_TEXTURE_STATIC );
            
            txData->SetTexture( 0, txDesc );
            SetAsset( 0, LAsset( txDesc->GetName(), txAssetDescr, txDesc->GetSamplerState() ) );

            HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );

            return success;
        }

    }
    return false;
}

// *************************************
// 
IPixelShaderChannelPtr              DefaultParallaxMapPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
void                                DefaultParallaxMapPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    m_psc->PostUpdate();    
}

} // model
} // bv
