#include "stdafx.h"

#include "DefaultParallaxMapPlugin.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Assets/DefaultAssets.h"


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

    helper.CreatePluginModel();
    
    helper.CreatePSModel();
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
void DefaultParallaxMapPlugin::SetPrevPlugin( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );

    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );
    auto ctx = m_psc->GetRendererContext();
    ctx->cullCtx->enabled = false;
}

// *************************************
// 
DefaultParallaxMapPlugin::DefaultParallaxMapPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin< IPlugin >( name, uid, prev, model )
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
    return m_prevPlugin->IsValid();
}

// *************************************
// 
bool                            DefaultParallaxMapPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto txAssetDescr = QueryTypedDesc< TextureAssetDescConstPtr >( assetDescr );
    
    // FIXME: dodac tutaj API pozwalajace tez ustawiac parametry dodawanej tekstury (normalny load z dodatkowymi parametrami)
    if ( txAssetDescr != nullptr )
    {
        //FIXME: use some better API to handle resources in general and textures in this specific case
        auto txDesc = DefaultTextureDescriptor::LoadTexture( txAssetDescr, DefaultParallaxMapPluginDesc::TextureName() );

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
IPixelShaderChannelPtr              DefaultParallaxMapPlugin::GetPixelShaderChannel       () const
{
    return m_psc;
}

// *************************************
// 
void                                DefaultParallaxMapPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperPixelShaderChannel::PropagateUpdate( m_psc, m_prevPlugin );

    m_psc->PostUpdate();    
}

} // model
} // bv