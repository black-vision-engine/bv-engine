#include "stdafx.h"

#include "VideoInputPlugin.h"

#include "Engine/Models/Plugins/Channels/PixelShader/GPUTextureDescriptor.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IValue.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAsset.h"
#include "Assets/Input/VideoInput/VideoInputTextureAsset.h"
#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"

#include "Assets/DefaultAssets.h"


namespace bv {
namespace model {


const std::string        VideoInputPlugin::PARAMS::ALPHA    = "alpha";
const std::string        VideoInputPlugin::PARAMS::TX_MAT   = "txMat";




// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
VideoInputPluginDesc::VideoInputPluginDesc                          ()
    : BasePluginDescriptor( UID(), "video input", "vidin" )
{
}

// *******************************
//
IPluginPtr              VideoInputPluginDesc::CreatePlugin              ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< VideoInputPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   VideoInputPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );

    //Create all models
    auto model = helper.GetModel();

    helper.SetOrCreatePluginModel();

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( VideoInputPlugin::PARAMS::ALPHA, 1.f, true );

    helper.SetOrCreateVSModel();
    helper.AddTransformParam( VideoInputPlugin::PARAMS::TX_MAT, true );

    auto param = helper.GetModel()->GetVertexShaderChannelModel()->GetParameter( VideoInputPlugin::PARAMS::TX_MAT );
    SetParameterCenterMass( param, 0.0f, glm::vec3( 0.5, 0.5, 0.0 ) );

    return model;
}

// *******************************
//
std::string             VideoInputPluginDesc::UID                       ()
{
    return "VIDEO_INPUT_PLUGIN";
}

// *******************************
// 
std::string             VideoInputPluginDesc::TextureName               ()
{
    return "VideoInput0";
}



// ************************************************************************* PLUGIN *************************************************************************

// *************************************
// 
bool                    VideoInputPlugin::SetPrevPlugin                 ( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );
    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );

    return true;
}

// ***********************
//
std::string             VideoInputPlugin::GetTextureName                ( UInt32 idx ) const
{
    if( idx == 0 )
        return VideoInputPluginDesc::TextureName();
    return "";
}

// *************************************
// 
VideoInputPlugin::VideoInputPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : TexturePluginBase( name, uid, prev, model )
    , m_vsc( nullptr )
{
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );

    SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< VideoInputAssetDesc >() );
}

// *************************************
// 
VideoInputPlugin::~VideoInputPlugin         ()
{}

// *************************************
// 
bool							VideoInputPlugin::IsValid     () const
{
    return ( GetPrevPlugin()->IsValid() );
}

// *************************************
// 
bool                            VideoInputPlugin::LoadResource  ( AssetDescConstPtr assetDescr )
{
    auto videoAssetDesc = QueryTypedDesc< VideoInputAssetDescConstPtr >( assetDescr );
    if( videoAssetDesc )
    {
        SamplerStateModelPtr newSamplerStateModel = CreateSamplerReplacment();

        auto videoAsset = std::static_pointer_cast< const VideoInputAsset >( AssetManager::GetInstance().LoadAsset( videoAssetDesc ) );
        if( videoAsset )
        {
            auto name = GetTextureName( 0 );

            auto texDesc = std::make_shared< GPUTextureDescriptor >( videoAsset->GetFillAsset(), name );

            texDesc->SetSamplerState( newSamplerStateModel );
            texDesc->SetName( name );

            ReplaceTexture( videoAssetDesc, texDesc, 0 );

            return true;
        }
    }

    return false;
}


// *************************************
// 
IVertexShaderChannelConstPtr        VideoInputPlugin::GetVertexShaderChannel      () const
{
    return m_vsc;
}

// *************************************
// 
void                                VideoInputPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, VideoInputPlugin::PARAMS::TX_MAT );
    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
}




} // model
} // bv


