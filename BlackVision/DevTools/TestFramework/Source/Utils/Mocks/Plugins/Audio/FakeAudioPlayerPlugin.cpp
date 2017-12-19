#include "stdafx.h"

#include "FakeAudioPlayerPlugin.h"

#include "Engine/Models/Plugins/Parameters/ParametersFactory.h"
#include "Engine/Models/Plugins/Channels/Geometry/AttributeChannelTyped.h"

#include "Engine/Models/Plugins/Channels/PixelShader/DefaultVideoStreamDescriptor.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"

#include "Engine/Models/Plugins/Simple/AVStreamDecoder/FFmpeg/FFmpegAVDecoder.h"

#include "Assets/DefaultAssets.h"

#include "Engine/Events/EventHandlerHelpers.h"
#include "Engine/Events/Events.h"

#include "Util/Audio/AudioUtils.h"


namespace bv {
namespace model {


const std::string        FakeAudioPlayerPlugin::PARAM::LOOP_COUNT = "loopCount";
const std::string        FakeAudioPlayerPlugin::PARAM::LOOP_ENABLED = "loopEnabled";
const std::string        FakeAudioPlayerPlugin::PARAM::GAIN = "gain";




// ************************************************************************* DESCRIPTOR *************************************************************************

// *******************************
//
FakeAudioPlayerPluginDesc::FakeAudioPlayerPluginDesc		()
    : BasePluginDescriptor( UID(), "FakeAudioPlugin" )
{
}

// *******************************
//
IPluginPtr                      FakeAudioPlayerPluginDesc::CreatePlugin           ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const
{
    return CreatePluginTyped< FakeAudioPlayerPlugin >( name, prev, timeEvaluator );
}

// *******************************
//
DefaultPluginParamValModelPtr   FakeAudioPlayerPluginDesc::CreateDefaultModel( ITimeEvaluatorPtr timeEvaluator ) const
{
    ModelHelper helper( timeEvaluator );
    auto model = helper.GetModel();

    helper.SetOrCreatePluginModel();
    helper.AddSimpleParam( FakeAudioPlayerPlugin::PARAM::LOOP_COUNT, std::numeric_limits< int >::max(), false );
    helper.AddSimpleParam( FakeAudioPlayerPlugin::PARAM::LOOP_ENABLED, true, false );
    helper.AddSimpleParam( FakeAudioPlayerPlugin::PARAM::GAIN, 1.f );

    return model;
}

// *******************************
//
std::string                 FakeAudioPlayerPluginDesc::UID              ()
{
    return "FAKE_AUDIO_PLUGIN";
}

// *******************************
//
std::string                 FakeAudioPlayerPluginDesc::AudioName        ()
{
    return "Audio0";
}

// ************************************************************************* PLUGIN *************************************************************************

bool					    FakeAudioPlayerPlugin::SetPrevPlugin        ( IPluginPtr prev )
{
    return BasePlugin::SetPrevPlugin( prev );
}

// *************************************
// 
FakeAudioPlayerPlugin::FakeAudioPlayerPlugin				        ( const std::string & name, const std::string & uid, IPluginPtr prev, DefaultPluginParamValModelPtr model )
    : BasePlugin( name, uid, prev, model )
    , m_fps( 50 )
{
    m_audioChannel = DefaultAudioChannel::Create( 48000, AudioFormat::STEREO16 ); // Default video card format. It doesn't require converting.

    SetPrevPlugin( prev );

    m_loopEnabledParam = QueryTypedParam< ParamBoolPtr >( GetParameter( PARAM::LOOP_ENABLED ) );
    m_loopCountParam = QueryTypedParam< ParamIntPtr >( GetParameter( PARAM::LOOP_COUNT ) );

    m_gainParam = QueryTypedParam< ParamFloatPtr >( GetParameter( PARAM::GAIN ) );
}

// *************************************
// 
FakeAudioPlayerPlugin::~FakeAudioPlayerPlugin				        ()
{}

// *************************************
// 
bool							FakeAudioPlayerPlugin::IsValid          () const
{
    return GetPrevPlugin()->IsValid();
}


// *************************************
// 
IAudioChannelPtr                    FakeAudioPlayerPlugin::GetAudioChannel          () const
{
    return m_audioChannel;
}

// *************************************
// 
void                                FakeAudioPlayerPlugin::Update                   ( TimeType t )
{
    BasePlugin::Update( t );
    UploadAudioFrame();
}


// *************************************
//
void                                FakeAudioPlayerPlugin::UploadAudioFrame         ()
{
    //update audio data
    auto chunk = GenerateNextFrame();
    if( chunk )
    {
        m_audioChannel->PushPacket( ApplyGain( chunk ) );
    }
}

// ***********************
//
SizeType                            FakeAudioPlayerPlugin::ComputeFrameSamples      ()
{
    auto samplesPerSec = m_audioChannel->GetFrequency();

    return SizeType( ceil( ( double)samplesPerSec / (double)m_fps ) );
}

// ***********************
//
MemoryChunkPtr                      FakeAudioPlayerPlugin::GenerateNextFrame        ()
{
    if( m_loopEnabledParam->Evaluate() )
    {
        if( m_loopCount > 0 && m_signalSource )
        {
            Int64 samplesPerFrame = ComputeFrameSamples();
            auto sampleSize = 2;        ///< @todo We should support mocking other formats.
            Int64 bytesPerFrame = samplesPerFrame * sampleSize;

            auto outData = MemoryChunk::Create( bytesPerFrame );
            Int64 chunkOffset = 0;

            while( bytesPerFrame > 0 )
            {
                Int64 toCopy = m_signalSource->Size() - m_signalOffset;
                toCopy = toCopy > bytesPerFrame ? bytesPerFrame : toCopy;

                memcpy( outData->GetWritable() + chunkOffset, m_signalSource->Get() + m_signalOffset, toCopy );

                m_signalOffset = ( toCopy + m_signalOffset >= m_signalSource->Size() ) ? 0 : m_signalOffset + toCopy;
                bytesPerFrame -= toCopy;
                chunkOffset += toCopy;
            }

            m_loopCount--;

            return outData;
        }
    }

    return MemoryChunkPtr();
}

// *************************************
//
MemoryChunkPtr						FakeAudioPlayerPlugin::ApplyGain				( const MemoryChunkPtr & audioFrameData ) const
{
    auto size = audioFrameData->Size();

    auto outData = MemoryChunk::Create( audioFrameData->Size() );

    audio::AudioUtils::ApplyGain( outData->GetWritable(), audioFrameData->Get(), size, m_gainParam->Evaluate() );

    return outData;
}

// ***********************
//
void                                FakeAudioPlayerPlugin::SetSignalSource              ( MemoryChunkPtr signal )
{
    m_signalSource = signal;

    m_loopCount = m_loopCountParam->Evaluate();
    m_signalOffset = 0;
}


} //model
} //bv