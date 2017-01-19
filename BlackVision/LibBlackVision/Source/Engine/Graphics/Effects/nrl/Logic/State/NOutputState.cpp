#include "stdafx.h"

#include "NOutputState.h"


namespace bv { namespace nrl {

// **************************
//
NOutputState::NOutputState                                  ( unsigned int w, unsigned int h )
    : m_width( w )
    , m_height( h )
{
}

// **************************
//
void                NOutputState::SetWidth                  ( unsigned int w )
{
    m_width = w;
}

// **************************
//
void                NOutputState::SetHeight                 ( unsigned int h )
{
    m_height = h;
}

// **************************
//
void                NOutputState::SetChannelMask            ( unsigned int channelMask )
{
    m_channelMask = channelMask;
}

// **************************
//
void                NOutputState::SetChannelMask            ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
{
    SetRIdx( rIdx );
    SetGIdx( gIdx );
    SetBIdx( bIdx );
    SetAIdx( aIdx );
}

// **************************
//
void                NOutputState::SetRIdx                   ( unsigned char rIdx )
{
    SetMaskedUChar( rIdx, 0 );
}

// **************************
//
void                NOutputState::SetGIdx                   ( unsigned char gIdx )
{
    SetMaskedUChar( gIdx, 2 );
}

// **************************
//
void                NOutputState::SetBIdx                   ( unsigned char bIdx )
{
    SetMaskedUChar( bIdx, 4 );
}

// **************************
//
void                NOutputState::SetAIdx                   ( unsigned char aIdx )
{
    SetMaskedUChar( aIdx, 6 );
}

// **************************
//
void                NOutputState::SetActiveRenderChannel    ( RenderChannelType activeRenderChannel )
{
    m_activeRenderChannel = activeRenderChannel;
}

// **************************
//
unsigned int        NOutputState::GetWidth                  () const
{
    return m_width;
}

// **************************
//
unsigned int        NOutputState::GetHeight                 () const
{
    return m_height;
}

// **************************
//
unsigned int        NOutputState::GetChannelMask            () const
{
    return m_channelMask;
}

// **************************
//
unsigned int        NOutputState::GetRIdx                   () const
{
    return GetMaskedUChar( 0 );
}

// **************************
//
unsigned int        NOutputState::GetGIdx                   () const
{
    return GetMaskedUChar( 2 );
}

// **************************
//
unsigned int        NOutputState::GetBIdx                   () const
{
    return GetMaskedUChar( 4 );
}

// **************************
//
unsigned int        NOutputState::GetAIdx                   () const
{
    return GetMaskedUChar( 6 );
}

// **************************
//
RenderChannelType   NOutputState::GetActiveRenderChannel    () const
{
    return m_activeRenderChannel;
}

// **************************
//
void                NOutputState::SetMaskedUChar            ( unsigned int val, unsigned int numBits )
{
    m_channelMask = ( m_channelMask & ~( 0xFF << numBits ) ) | ( val & 0x3 ) << numBits;
}

// **************************
//
unsigned int        NOutputState::GetMaskedUChar          ( unsigned int numBits ) const
{
    return ( m_channelMask & ( 0xFF << numBits ) ) >> numBits;
}

} //nrl
} //bv
