#include "stdafx.h"
#include "AudioMixer.h"
#include "AudioUtils.h"

namespace bv { namespace audio
{

// *********************************
//
AudioMixer::AudioMixer      ( const audio::AudioRenderChannelData & arcd )
    : m_arcd( arcd )
{}

// *********************************
//
bool AudioMixer::MixAudioBuffersVecs           ( const MemoryChunkPtr & output ) const
{
    // check whether any data needs uploading
    if( IsAnyBufferReady( output->Size() ) )
        output->Clear();
    
    for( SizeType i = 0; i < m_arcd.NumSources(); ++i )
        MixAudioBuffers( output, m_arcd.GetData( i ) );
    
    //auto size = output->Size();
    //auto outData = MemoryChunk::Create( size );
    //
    //AudioUtils::ApplyGain( outData->GetWritable(), output->Get(), size, m_gain );
    //
    //output = outData;

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
void AudioMixer::MixAudioBuffers            ( const MemoryChunkPtr & output, const AudioBufferVec & audioBuffs ) const
{
    auto rawData = output->GetWritable();
    SizeType rewriteSize = 0;

    for( auto & ab : audioBuffs )
    {
        auto inMC = ab->GetData();
        auto inSize = inMC->Size();
        
        AudioUtils::MixAudio16( rawData, inMC->Get(), inSize );
        rawData += inSize;

        rewriteSize += inSize;
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
