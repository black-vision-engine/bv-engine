#include "stdafx.h"

#include "DefaultAudioDecoderPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultVideoStreamDescriptor.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/FFmpegAVDecoder.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Events/Events.h"

#include "Engine/Audio/Resources/AudioUtils.h"


namespace bv { namespace model {


const std::string        DefaultAudioDecoderPlugin::PARAM::DECODER_STATE  = "state";
const std::string        DefaultAudioDecoderPlugin::PARAM::SEEK_OFFSET    = "offset";
const std::string        DefaultAudioDecoderPlugin::PARAM::LOOP_ENABLED   = "loopEnabled";
const std::string        DefaultAudioDecoderPlugin::PARAM::LOOP_COUNT     = "loopCount";
const std::string        DefaultAudioDecoderPlugin::PARAM::GAIN			  = "gain";

typedef ParamEnum< DefaultAudioDecoderPlugin::DecoderMode > ParamEnumDM;


// ***********************
//
template<>
static IParameterPtr    ParametersFactory::CreateTypedParameter< DefaultAudioDecoderPlugin::DecoderMode > ( const std::string & name, ITimeEvaluatorPtr timeline )
{
    return CreateParameterEnum< DefaultAudioDecoderPlugin::DecoderMode >( name, timeline );
}

// ***********************
//
VoidPtr    ParamEnumDM::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}




// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
DefaultAudioDecoderPluginDesc::DefaultAudioDecoderPluginDesc		()
    : BasePluginDescriptor( UID(), "audio_decoder" )
{
}

// *******************************
//
IPluginPtr                      DefaultAudioDecoderPluginDesc::CreatePlugin           ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< DefaultAudioDecoderPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   DefaultAudioDecoderPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );
    auto model  = helper.GetModel();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( DefaultAudioDecoderPlugin::PARAM::SEEK_OFFSET, 0.f, true, true );
    helper.AddParam< IntInterpolator, DefaultAudioDecoderPlugin::DecoderMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumDM >
        ( DefaultAudioDecoderPlugin::PARAM::DECODER_STATE, DefaultAudioDecoderPlugin::DecoderMode::STOP, true, true );
    helper.AddSimpleParam( DefaultAudioDecoderPlugin::PARAM::LOOP_ENABLED, false, false );
    helper.AddSimpleParam( DefaultAudioDecoderPlugin::PARAM::LOOP_COUNT, 0, true, true );
	helper.AddSimpleParam( DefaultAudioDecoderPlugin::PARAM::GAIN, 1.f );

    return model;
}

// *******************************
//
std::string                 DefaultAudioDecoderPluginDesc::UID              ()
{
    return "DEFAULT_AUDIO_DECODER";
}

// *******************************
//
std::string                 DefaultAudioDecoderPluginDesc::AudioName        ()
{
    return "Audio0";
}

// ************************************************************************* PLUGIN *************************************************************************

bool					    DefaultAudioDecoderPlugin::SetPrevPlugin        ( IPluginPtr prev )
{
    return BasePlugin::SetPrevPlugin( prev );
}

// *************************************
// 
DefaultAudioDecoderPlugin::DefaultAudioDecoderPlugin				        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_decoder( nullptr )
    , m_prevDecoderModeTime( 0 )
    , m_prevOffsetTime( 0 )
    , m_isFinished( false )
{
	m_audioChannel = DefaultAudioChannel::Create( 48000, AudioFormat::STEREO16 ); // Default video card format. It doesn't require converting.

    SetPrevPlugin( prev );

    m_decoderModeParam = QueryTypedParam< std::shared_ptr< ParamEnum< DecoderMode > > >( GetParameter( PARAM::DECODER_STATE ) );
    m_decoderModeParam->SetGlobalCurveType( CurveType::CT_POINT );
    
    m_offsetParam = QueryTypedParam< ParamFloatPtr >( GetParameter( PARAM::SEEK_OFFSET ) );
    m_offsetParam->SetGlobalCurveType( CurveType::CT_POINT );

    m_loopEnabledParam = QueryTypedParam< ParamBoolPtr >( GetParameter( PARAM::LOOP_ENABLED ) );
    m_loopCountParam = QueryTypedParam< ParamIntPtr >( GetParameter( PARAM::LOOP_COUNT ) );

	m_gainParam = QueryTypedParam< ParamFloatPtr >( GetParameter( PARAM::GAIN ) );

    m_decoderMode =  m_decoderModeParam->Evaluate();

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< AVAssetDesc >() );
}

// *************************************
// 
DefaultAudioDecoderPlugin::~DefaultAudioDecoderPlugin				        ()
{
}

// *************************************
// 
bool							DefaultAudioDecoderPlugin::IsValid          () const
{
    return GetPrevPlugin()->IsValid();
}

// *************************************
// 
bool                            DefaultAudioDecoderPlugin::LoadResource		( AssetDescConstPtr assetDescr )
{
    m_assetDesc = QueryTypedDesc< AVAssetDescConstPtr >( assetDescr );

    if ( m_assetDesc )
    {
        auto asset = LoadTypedAsset< AVAsset >( assetDescr );
        if( asset )
        {
            m_decoder = std::make_shared< FFmpegAVDecoder >( asset );

            if( m_decoder->HasAudio() )
            {
                m_audioChannel->SetFrequency( m_decoder->GetSampleRate() );
                m_audioChannel->SetFormat( m_decoder->GetAudioFormat() );

                std::static_pointer_cast< FFmpegAVDecoder >( m_decoder )->ProcessFirstAVFrame();

                SetAsset( 0, LAsset( DefaultAudioDecoderPluginDesc::AudioName(), assetDescr, nullptr ) );

                UpdateDecoderState( m_decoderMode );

                return true;
            }
        }
    }

    return false;
}

// *************************************
// 
IAudioChannelPtr                    DefaultAudioDecoderPlugin::GetAudioChannel          () const
{
    return m_audioChannel;
}

// *************************************
// 
void                                DefaultAudioDecoderPlugin::Update                   ( TimeType t )
{
    BasePlugin::Update( t );

    UpdateDecoder();
    UploadAudioFrame();
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::UpdateDecoder            ()
{
    if( m_decoder )
    {
        m_decoderMode = m_decoderModeParam->Evaluate();

        if( ParameterChanged( PARAM::DECODER_STATE ) )
        {
            UpdateDecoderState( m_decoderMode );
        }

        if( ParameterChanged( PARAM::SEEK_OFFSET ) )
        {
            m_decoder->Seek( m_offsetParam->Evaluate() );

            std::static_pointer_cast< FFmpegAVDecoder >( m_decoder )->ProcessFirstAVFrame();
            UpdateDecoderState( m_decoderMode );
        }

        HandlePerfectLoops();

        // send event on video finished
        if( !m_isFinished && m_decoder->IsFinished() && m_assetDesc )
        {
            m_decoderModeParam->SetVal( DecoderMode::STOP, 0.f );

            BroadcastHasFinishedEvent();
            m_isFinished = true;
            TriggerAudioEvent( AssetTrackerInternalEvent::Command::EOFAudio );
        }
    }
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::Play                     ()
{
    m_decoder->Play();
    m_isFinished = false;
    TriggerAudioEvent( AssetTrackerInternalEvent::Command::PlayAudio );
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::Stop                     ()
{
    m_decoder->Stop();
    m_decoder->Seek( 0.f );
    TriggerAudioEvent( AssetTrackerInternalEvent::Command::StopAudio );
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::Pause                    ()
{
    m_decoder->Pause();
    TriggerAudioEvent( AssetTrackerInternalEvent::Command::PauseAudio );
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::HandlePerfectLoops       ()
{
    m_decoderMode = m_decoderModeParam->Evaluate();
    if( m_decoderMode == DecoderMode::RESTART )
    {
        UpdateDecoderState( DecoderMode::STOP );
        m_decoderModeParam->SetVal( DecoderMode::PLAY, 0.f );
    }
    else
    {
        auto loopEnabled = m_loopEnabledParam->Evaluate();
        auto loopCount = m_loopCountParam->Evaluate();
        if( ParameterChanged( PARAM::LOOP_COUNT ) )
        {
            if( loopCount == 0 )
                loopCount = std::numeric_limits< Int32 >::max(); // set 'infinite' loop

            m_loopCount = loopCount;
        }


        if( m_decoder->IsFinished() )
        {
            m_decoder->Seek( 0.f );

            if( loopEnabled && m_loopCount > 1 )
                m_loopCount--;
            else
            {
                m_decoderModeParam->SetVal( DecoderMode::STOP, 0.f );
                m_decoderMode = m_decoderModeParam->Evaluate();
            }

            UpdateDecoderState( m_decoderMode );
        }
    }
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::UpdateDecoderState       ( DecoderMode mode )
{
    if( mode == DecoderMode::RESTART )
    {
        UpdateDecoderState( DecoderMode::STOP );
        m_decoderModeParam->SetVal( DecoderMode::PLAY, 0.f );
    }

    switch( mode )
    {
        case DecoderMode::PLAY:
            Play(); break;
        case DecoderMode::STOP:
            Stop(); break;
        case DecoderMode::PAUSE:
            Pause(); break;
    }
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::UploadAudioFrame         ()
{
    //update audio data
    AVMediaData mediaData;
	while( m_decoder->GetAudioMediaData( mediaData ) )
    {
        m_audioChannel->PushPacket( ApplyGain( mediaData.frameData ) );
    }
}

// *************************************
//
MemoryChunkPtr						DefaultAudioDecoderPlugin::ApplyGain				( const MemoryChunkPtr & audioFrameData ) const
{
	auto size = audioFrameData->Size();

	auto outData = MemoryChunk::Create( audioFrameData->Size() );

	audio::AudioUtils::ApplyGain( outData->GetWritable(), audioFrameData->Get(), size, m_gainParam->Evaluate() );

	return outData;
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::BroadcastHasFinishedEvent    ()
{
    auto evt = std::make_shared< VideoDecoderEvent >();
    evt->AssetPath = m_assetDesc->GetStreamPath();
    evt->EventCommand = VideoDecoderEvent::Command::HasFinished;
    JsonSerializeObject ser;
    evt->Serialize( ser );
    SendResponse( ser, SEND_BROADCAST_EVENT, 0 );
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::TriggerAudioEvent            ( AssetTrackerInternalEvent::Command command )
{
    auto evt = std::make_shared< AssetTrackerInternalEvent >( command );
    evt->PluginOwner = this;
    GetDefaultEventManager().TriggerEvent( evt );
}

} //model
} //bv