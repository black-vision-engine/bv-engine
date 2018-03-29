#include "stdafx.h"

#include "VideoInputPlugin.h"

#include "Engine/Models/Plugins/Channels/PixelShader/GPUTextureDescriptor.h"

#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Interfaces/IValue.h"

#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Engine/Models/Plugins/Channels/HelperVertexShaderChannel.h"
#include "Engine/Models/Plugins/Channels/Geometry/HelperVertexAttributesChannel.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Util/Audio/AudioUtils.h"

#include "Assets/Input/VideoInput/VideoInputAssetDesc.h"
#include "Assets/Input/VideoInput/VideoInputAsset.h"
#include "Assets/Input/VideoInput/VideoInputTextureAsset.h"
#include "Assets/Input/VideoInput/VideoInputTextureAssetDesc.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Models/Plugins/Simple/SpecialPlugins/BlendHelper.h"


namespace bv {
namespace model {


const std::string        VideoInputPlugin::PARAMS::ALPHA        = "alpha";
const std::string        VideoInputPlugin::PARAMS::TX_MAT       = "txMat";
const std::string        VideoInputPlugin::PARAMS::GAIN         = "gain";
const std::string        VideoInputPlugin::PARAMS::ENABLE_KEY   = "enableKey";




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
    helper.AddSimpleParam( VideoInputPlugin::PARAMS::GAIN, 1.f, true );

    helper.SetOrCreatePSModel();
    helper.AddSimpleParam( VideoInputPlugin::PARAMS::ALPHA, 1.f, true );
    helper.AddSimpleParam( VideoInputPlugin::PARAMS::ENABLE_KEY, false, true );

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
std::string             VideoInputPluginDesc::FillTextureName           ()
{
    return "VideoInputFill";
}

// *******************************
// 
std::string             VideoInputPluginDesc::KeyTextureName            ()
{
    return "VideoInputKey";
}


// ************************************************************************* PLUGIN *************************************************************************

// ***********************
//
void                    VideoInputPlugin::InitVertexAttributesChannel   ()
{
    if( !( GetPrevPlugin() && GetPrevPlugin()->GetVertexAttributesChannel() ) )
    {
        m_vaChannel = nullptr;
        return;
    }

    m_vaChannel = InitAttributesChannelWithUVs( m_vaChannel );

    GenerateDefaultUVs( m_vaChannel, GetPrevPlugin()->GetVertexAttributesChannel() );
}

// *************************************
// 
bool                    VideoInputPlugin::SetPrevPlugin                 ( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );
    HelperPixelShaderChannel::CloneRenderContext( m_psc, prev );

    auto ctx = m_psc->GetRendererContext();

    ctx->alphaCtx->blendEnabled = true;
    BlendHelper::SetBlendColorContext( ctx, BlendHelper::BlendMode::BM_Alpha );

    return true;
}

// ***********************
//
std::string             VideoInputPlugin::GetTextureName                ( UInt32 idx ) const
{
    if( idx == 0 )
        return VideoInputPluginDesc::FillTextureName();
    else if( idx == 1 )
        return VideoInputPluginDesc::KeyTextureName();
    return "";
}

// *************************************
// 
VideoInputPlugin::VideoInputPlugin         ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : TexturePluginBase( name, uid, prev, model )
    , m_vsc( nullptr )
    , m_lastAudioUpdateID( 0 )
{
    m_vsc = DefaultVertexShaderChannel::Create( model->GetVertexShaderChannelModel() );
    m_audioChannel = DefaultAudioChannel::Create( 48000, AudioFormat::STEREO16 );       // Default video card format. It doesn't require converting.

    SetPrevPlugin( prev );

    m_gainValue = QueryTypedValue< ValueFloatPtr >( GetValue( PARAMS::GAIN ) );

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
        auto videoAsset = std::static_pointer_cast< const VideoInputAsset >( AssetManager::GetInstance().LoadAsset( videoAssetDesc ) );
        if( videoAsset )
        {
            LoadVideoInputTexture( videoAsset, videoAssetDesc );
            LoadVideoInputAudio( videoAsset, videoAssetDesc );

            return true;
        }
    }

    return false;
}

// ***********************
//
void                                VideoInputPlugin::LoadVideoInputTexture          ( VideoInputAssetConstPtr videoAsset, AssetDescConstPtr videoAssetDesc )
{
    m_videoInputAsset = videoAsset;

    LoadVideoInputTexture( videoAsset->GetFillAsset(), videoAssetDesc, 0 );
    LoadVideoInputTexture( videoAsset->GetKeyAsset(), videoAssetDesc, 1 );

    // We set two textures but only one asset exists. That's why we use name and sampler from first texture.
    SetAsset( 0, LAsset( GetTextureName( 0 ), videoAssetDesc, CreateSamplerReplacment( 0 ) ) );
}

// ***********************
//
void                                VideoInputPlugin::LoadVideoInputTexture     ( VideoInputTextureAssetConstPtr videoAsset, AssetDescConstPtr videoAssetDesc, UInt32 idx )
{
    // Use the same sampler for both textures.
    SamplerStateModelPtr newSamplerStateModel = CreateSamplerReplacment( 0 );

    auto name = GetTextureName( idx );
    auto texDesc = std::make_shared< GPUTextureDescriptor >( videoAsset, name );

    texDesc->SetSamplerState( newSamplerStateModel );
    texDesc->SetName( name );

    ReplaceVideoTexture( videoAssetDesc, texDesc, idx );
}

// ***********************
//
void                                VideoInputPlugin::LoadVideoInputAudio       ( VideoInputAssetConstPtr videoAsset, AssetDescConstPtr desc )
{
    auto audioInput = videoAsset->GetAudio();
    if( audioInput )
    {
        m_audioChannel->SetFrequency( audioInput->GetFrequency() );
        m_audioChannel->SetFormat( audioInput->GetFormat() );

        TriggerAudioEvent( AssetTrackerInternalEvent::Command::PlayAudio );
    }
    else
    {
        TriggerAudioEvent( AssetTrackerInternalEvent::Command::StopAudio );
    }
}

// ***********************
//
void                                VideoInputPlugin::ReplaceVideoTexture       ( const AssetDescConstPtr &, ITextureDescriptorPtr texDesc, UInt32 texIdx )
{
    auto txData = m_psc->GetTexturesDataImpl();

    txData->SetTexture( texIdx, texDesc );
    //SetAsset( texIdx, LAsset( texDesc->GetName(), assetDesc, texDesc->GetSamplerState() ) );

    HelperPixelShaderChannel::SetTexturesDataUpdate( m_psc );
}

// *************************************
// 
IVertexShaderChannelConstPtr        VideoInputPlugin::GetVertexShaderChannel    () const
{
    return m_vsc;
}

// *************************************
// 
IAudioChannelPtr                    VideoInputPlugin::GetAudioChannel           () const
{
    return m_audioChannel;
}

// *************************************
// 
void                                VideoInputPlugin::Update                      ( TimeType t )
{
    BasePlugin::Update( t );

    HelperVertexShaderChannel::InverseTextureMatrix( m_pluginParamValModel, VideoInputPlugin::PARAMS::TX_MAT );
    HelperPixelShaderChannel::PropagateUpdate( m_psc, GetPrevPlugin() );

    HelperVertexAttributesChannel::PropagateAttributesUpdate( m_vaChannel, GetPrevPlugin() );
    if( HelperVertexAttributesChannel::PropagateTopologyUpdate( m_vaChannel, GetPrevPlugin() ) )
    {
        InitVertexAttributesChannel();
    }

    UpdateAudio();

    m_vsc->PostUpdate();
    m_psc->PostUpdate();
}

// ***********************
//
void                                VideoInputPlugin::UpdateAudio                   ()
{
    auto videoInputAudio = m_videoInputAsset->GetAudio();
    if( videoInputAudio && m_lastAudioUpdateID < videoInputAudio->LastAudioUpdate() )
    {
        auto audioFrame = videoInputAudio->GetFrame();
        if( audioFrame )
            m_audioChannel->PushPacket( ApplyGain( audioFrame ) );

        m_lastAudioUpdateID = videoInputAudio->LastAudioUpdate();
    }
}

// ***********************
//
MemoryChunkPtr                      VideoInputPlugin::ApplyGain                     ( MemoryChunkConstPtr audioFrameData ) const
{
    auto size = audioFrameData->Size();
    auto outData = MemoryChunk::Create( audioFrameData->Size() );

    audio::AudioUtils::ApplyGain( outData->GetWritable(), audioFrameData->Get(), size, m_gainValue->GetValue() );

    return outData;
}

// *************************************
//
void                                VideoInputPlugin::TriggerAudioEvent            ( AssetTrackerInternalEvent::Command command )
{
    auto evt = std::make_shared< AssetTrackerInternalEvent >( command );
    evt->PluginOwner = this;
    GetDefaultEventManager().TriggerEvent( evt );
}


} // model
} // bv


