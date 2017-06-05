#include "stdafx.h"
#include "AudioMixer.h"
#include "AudioUtils.h"

namespace bv { namespace audio
{

// *********************************
//
AudioMixer::AudioMixer      ( const audio::AudioRenderChannelData & arcd )
    : m_arcd( arcd )
    , m_gain( 1.f )
{}

// *********************************
//
bool AudioMixer::MixAudioBuffersVecs           ( const MemoryChunkPtr & output ) const
{
    // check whether any data needs uploading
    if( IsAnyBufferReady( output->Size() ) )
        output->Clear();
    
    if( m_gain == 0.f )
    {
        output->Clear();
        return true;
    }

    for( SizeType i = 0; i < m_arcd.NumSources(); ++i )
        MixAudioBuffers( output, m_arcd.GetData( i ) );
    
    if( m_gain < 1.f && m_gain > 0.f )
        AudioUtils::ApplyGain( output->GetWritable(), output->Size(), m_gain );

    return true;
}

// *********************************
//
bool AudioMixer::IsAnyBufferReady             ( SizeType requestedBufferSize ) const
{
    for( SizeType i = 0; i < m_arcd.NumSources(); ++i )
    {
        if( !m_arcd.GetData( i ).back()->IsEOF() )
        {
            if( requestedBufferSize <= m_arcd.DataSize( i ) )
                return true;
        }
        else
        {
            if( m_arcd.DataSize( i ) > 0 )
                return true;
        }
    }

    return false;
}

// *********************************
//
void AudioMixer::SetGain                    ( Float32 gain )
{
    m_gain = gain;
}

// *********************************
//
void AudioMixer::MixAudioBuffers            ( const MemoryChunkPtr & output, const AudioBufferVec & audioBuffs ) const
{
    auto rawData = output->GetWritable();
    auto outSize = output->Size();
    SizeType rewriteSize = 0;

    for( auto & ab : audioBuffs )
    {
        auto inMC = ab->GetData();
        auto inSize = inMC->Size();
        
        auto minSize = std::min( outSize - rewriteSize, inSize );

        AudioUtils::MixAudio16( rawData, inMC->Get(), minSize );

        rawData += minSize;

        rewriteSize += minSize;

        if( rewriteSize == outSize )
            break;
    }
}

//// *********************************
////
//bool                    AudioMixer::IsAnySourcePlaying           () const
//{
//    for( auto & obj : m_sources )
//    {
//        if( obj.first->IsPlaying() )
//        {
//            return true;
//        }
//    }
//    return false;
//}



} // audio
} // bv
