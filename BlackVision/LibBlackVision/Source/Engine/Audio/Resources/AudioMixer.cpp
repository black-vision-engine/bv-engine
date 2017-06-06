#include "stdafx.h"
#include "AudioMixer.h"
#include "AudioUtils.h"

namespace bv { namespace audio
{

// *********************************
//
AudioMixer::AudioMixer      ()
    : m_gain( 1.f )
{}

// *********************************
//
bool AudioMixer::PopAndMixAudioData           ( const MemoryChunkPtr & output )
{
    // check whether any data needs uploading
    if( IsAnyBufferReady( output->Size() ) )
        output->Clear();
    
    if( m_gain == 0.f )
    {
        output->Clear();
        return true;
    }

    for( SizeType i = 0; i < m_audioBuffersSources.size(); ++i )
        MixAudioBuffers( output, i );
    
    if( m_gain < 1.f && m_gain > 0.f )
        AudioUtils::ApplyGain( output->GetWritable(), output->Size(), m_gain );

    return true;
}

// *********************************
//
bool AudioMixer::IsAnyBufferReady             ( SizeType requestedBufferSize ) const
{
    for( SizeType i = 0; i < m_audioBuffersSources.size(); ++i )
    {
        if( !m_eofVec[ i ] )
        {
            if( requestedBufferSize <= m_sizes[ i ] )
                return true;
        }
        else
        {
            if( m_sizes[ i ] > 0 )
                return true;
        }
    }

    return false;
}

// *********************************
//
void AudioMixer::ResizeSources          ( SizeType numSources )
{
    m_audioBuffersSources.resize( numSources );
    m_eofVec.resize( numSources );
    m_sizes.resize( numSources );
}

// *********************************
//
void AudioMixer::PushData                   ( SizeType sourceIdx, const AudioBufferVec & audioBuffs )
{
    assert( sourceIdx < m_audioBuffersSources.size() );

    for( auto & ab : audioBuffs )
    {
        m_audioBuffersSources[ sourceIdx ].PushBack( ab->GetData() );
        m_sizes[ sourceIdx ] += ab->GetData()->Size();
    }

    if( !audioBuffs.empty() )
        m_eofVec[ sourceIdx ] = audioBuffs.back()->IsEOF();
    else
        m_eofVec[ sourceIdx ] = false;
}

// *********************************
//
void AudioMixer::SetGain                    ( Float32 gain )
{
    m_gain = gain;
}

// *********************************
//
bool AudioMixer::MixAudioBuffers            ( const MemoryChunkPtr & output, SizeType sourceIdx )
{
    auto dataSize = output->Size();
    auto dataOffset = ( SizeType )0;
    auto rawData = output->GetWritable();

    bool force = m_eofVec[ sourceIdx ];

    auto & bufferedDataSize = m_sizes[ sourceIdx ];

    if( bufferedDataSize >= dataSize || ( bufferedDataSize > 0 && force ) )
    {
        while( dataSize && bufferedDataSize && ( bufferedDataSize >= dataSize || force ) )
        {
            auto chunkData = m_audioBuffersSources[ sourceIdx ].Front();
            auto chunkRawData = chunkData->Get();
            auto chunkDataSize = chunkData->Size();

            auto rewriteSize = ( SizeType )std::min( chunkDataSize, dataSize );
          
            //FIXME: assmuption that input data is always signed short (default format)
            AudioUtils::MixAudio16( rawData + dataOffset, chunkRawData, rewriteSize );

            dataOffset += rewriteSize;
            dataSize -= rewriteSize;
            bufferedDataSize -= rewriteSize;
            m_audioBuffersSources[ sourceIdx ].PopFront();

            if( force )
                return true;

            if( chunkDataSize > rewriteSize )
            {
                auto remainingSize = chunkDataSize - rewriteSize;
                auto offsetChunkData = MemoryChunk::Create( remainingSize );
                auto offsetChunkRawData = offsetChunkData->GetWritable();

                memcpy( offsetChunkRawData, chunkData->Get() + rewriteSize, remainingSize );

                m_audioBuffersSources[ sourceIdx ].PushFront( offsetChunkData );
            }
        }

        //std::cout << this << "    " << sourceIdx << "    " << bufferedDataSize << std::endl;

        return true;
    }

    return false;
}

} // audio
} // bv
