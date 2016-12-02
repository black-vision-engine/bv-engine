#include "stdafx.h"

#include "DefaultAudioDecoderPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultVideoStreamDescriptor.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Events/EventHandlerHelpers.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {


const std::string        DefaultAudioDecoderPlugin::PARAM::DECODER_STATE  = "state";
const std::string        DefaultAudioDecoderPlugin::PARAM::SEEK_OFFSET    = "offset";
const std::string        DefaultAudioDecoderPlugin::PARAM::LOOP_ENABLED   = "loopEnabled";
const std::string        DefaultAudioDecoderPlugin::PARAM::LOOP_COUNT     = "loopCount";

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

#include "Engine/Models/Plugins/ParamValModel/SimpleParamValEvaluator.inl"


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
    helper.AddSimpleParam( DefaultAudioDecoderPlugin::PARAM::SEEK_OFFSET, glm::vec2( 0.f ), true );
    helper.AddParam< IntInterpolator, DefaultAudioDecoderPlugin::DecoderMode, ModelParamType::MPT_ENUM, ParamType::PT_ENUM, ParamEnumDM >
        ( DefaultAudioDecoderPlugin::PARAM::DECODER_STATE, DefaultAudioDecoderPlugin::DecoderMode::STOP, true, true );
    helper.AddSimpleParam( DefaultAudioDecoderPlugin::PARAM::LOOP_ENABLED, false, false );
    helper.AddSimpleParam( DefaultAudioDecoderPlugin::PARAM::LOOP_COUNT, 0, true, true );

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

void					    DefaultAudioDecoderPlugin::SetPrevPlugin        ( IPluginPtr prev )
{
    BasePlugin::SetPrevPlugin( prev );
}

// *************************************
// 
DefaultAudioDecoderPlugin::DefaultAudioDecoderPlugin				        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_decoder( nullptr )
    , m_prevOffsetCounter( 0 )
    , m_prevDecoderModeTime( 0 )
    , m_prevOffsetTime( 0 )
    , m_isFinished( false )
{
    m_audioChannel = DefaultAudioChannel::Create( 44100, AudioFormat::STEREO16 );

    SetPrevPlugin( prev );

    LoadResource( DefaultAssets::Instance().GetDefaultDesc< AVAssetDesc >() );

    m_decoderModeParam = QueryTypedParam< std::shared_ptr< ParamEnum< DecoderMode > > >( GetParameter( PARAM::DECODER_STATE ) );
    m_decoderModeParam->SetGlobalCurveType( CurveType::CT_POINT );
    
    m_offsetParam = QueryTypedParam< ParamVec2Ptr >( GetParameter( PARAM::SEEK_OFFSET ) );
    m_offsetParam->SetGlobalCurveType( CurveType::CT_POINT );

    m_loopEnabledParam = QueryTypedParam< ParamBoolPtr >( GetParameter( PARAM::LOOP_ENABLED ) );
    m_loopCountParam = QueryTypedParam< ParamIntPtr >( GetParameter( PARAM::LOOP_COUNT ) );

    m_decoderMode =  m_decoderModeParam->Evaluate();
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
    return m_prevPlugin->IsValid();
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
            m_decoder = nullptr;
            m_decoder = std::make_shared< FFmpegAVDecoder >( asset );

            if( m_decoder->HasAudio() )
            {
                m_audioChannel->SetFrequency( m_decoder->GetSampleRate() );
                m_audioChannel->SetFormat( m_decoder->GetAudioFormat() );

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

    MarkOffsetChanges();

    UpdateDecoder();
    UploadAudioFrame();
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::UpdateDecoder            ()
{
    if( m_decoder )
    {
        m_decoderMode =  m_decoderModeParam->Evaluate();

        if( ParameterChanged( PARAM::DECODER_STATE ) )
        {
            UpdateDecoderState( m_decoderMode );
        }

        // edge case - looped timeline
        auto decoderModeTime = m_decoderModeParam->GetLocalEvaluationTime();
        if( decoderModeTime < m_prevDecoderModeTime )
        {
            m_prevOffsetCounter = 0;
        }
        m_prevDecoderModeTime = decoderModeTime;

        // update offset 
        auto offset = m_offsetParam->Evaluate();
        auto offsetTime = m_offsetParam->GetLocalEvaluationTime();
        if( ( m_prevOffsetCounter != offset[ 1 ] ) || ( m_prevOffsetTime > offsetTime ) )
        {
            m_decoder->Seek( offset[ 0 ] );
            m_prevOffsetCounter = offset[ 1 ];
        }

        HandlePerfectLoops();

        // send event on video finished
        if( !m_isFinished && m_decoder->IsFinished() && m_assetDesc )
        {
            BroadcastHasFinishedEvent();
            m_isFinished = true;
        }
    }
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::Play                     ()
{
    m_decoder->Play();
    m_isFinished = false;
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::Stop                     ()
{
    m_decoder->Stop();
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
    auto loopEnabled = m_loopEnabledParam->Evaluate();
    auto loopCount = m_loopCountParam->Evaluate();
    if( ParameterChanged( PARAM::LOOP_COUNT ) )
    {
        if( loopCount == 0 )
        {
            loopCount = std::numeric_limits< Int32 >::max(); // set 'infinite' loop
        }
        m_loopCount = loopCount;
    }

    if( loopEnabled && m_decoder->IsEOF() && m_loopCount > 1 )
    {
        m_decoder->Seek( 0.f, false );     // do not clear buffer
        m_loopCount--;
    }
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::UpdateDecoderState       ( DecoderMode mode )
{
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
    if( m_decoder->GetAudioMediaData( mediaData ) )
    {
        m_audioChannel->PushPacket( mediaData.frameData );
    }
}

// *************************************
//
void                                DefaultAudioDecoderPlugin::MarkOffsetChanges        ()
{
    auto counter = 0;
    const auto keys = m_offsetParam->AccessInterpolator().GetKeys();
    for( auto & key : keys )
    {
        m_offsetParam->SetVal( glm::vec2( key.val[ 0 ], ++counter ), key.t );
    }
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