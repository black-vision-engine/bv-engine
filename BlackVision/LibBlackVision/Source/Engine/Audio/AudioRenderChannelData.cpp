#include "stdafx.h"
#include "AudioRenderChannelData.h"

namespace bv { namespace audio
{

// *******************************
//
void AudioRenderChannelData::ClearBuffers       ()
{
    m_audioBuffers.clear();
    m_sizes.clear();
}

// *******************************
//
void AudioRenderChannelData::PushData           ( const AudioBufferVec & chunks )
{
    m_audioBuffers.push_back( chunks );
    m_sizes.push_back( 0 );
    for( auto ch : chunks )
        m_sizes.back() += ch->GetSize();
}

// *******************************
//
void AudioRenderChannelData::PushData           ( const AudioBufferVec & chunks, SizeType sourceIdx )
{
    m_audioBuffers[ sourceIdx ] = chunks;
    m_sizes[ sourceIdx ] = 0;
    for( auto ch : chunks )
        m_sizes[ sourceIdx ] += ch->GetSize();
}

// *******************************
//
SizeType AudioRenderChannelData::NumSources     () const
{
    return m_audioBuffers.size();
}

// *******************************
//
AudioBufferVec AudioRenderChannelData::GetData  ( SizeType sourceIdx ) const
{
    return m_audioBuffers[ sourceIdx ];
}

// *******************************
//
SizeType AudioRenderChannelData::DataSize       ( SizeType sourceIdx ) const
{
    return m_sizes[ sourceIdx ];
}

} // audio
} // bv