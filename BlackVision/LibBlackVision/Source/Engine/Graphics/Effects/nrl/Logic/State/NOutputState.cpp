#include "stdafx.h"

#include "NOutputState.h"


namespace bv { namespace nrl {

// **************************
//
NOutputState::NOutputState                                      ( unsigned int w, unsigned int h )
    : m_width( w )
    , m_height( h )
    , m_channelMapping( 0 )
{
    SetChannelMapping( 0, 1, 2, 3 );

    SetMaskState( true, true, true, true );
}

// **************************
//
void                NOutputState::SetWidth                      ( unsigned int w )
{
    m_width = w;
}

// **************************
//
void                NOutputState::SetHeight                     ( unsigned int h )
{
    m_height = h;
}

// **************************
//
void                NOutputState::SetChannelMapping             ( unsigned int channelMapping )
{
    m_channelMapping = channelMapping;
}

// **************************
//
void                NOutputState::SetChannelMapping             ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
{
    m_channelMapping = 0;

    SetRIdx( rIdx );
    SetGIdx( gIdx );
    SetBIdx( bIdx );
    SetAIdx( aIdx );
}

// **************************
//
void                NOutputState::SetRIdx                       ( unsigned char rIdx )
{
    SetMappedUChar( rIdx, 0 );
}

// **************************
//
void                NOutputState::SetGIdx                       ( unsigned char gIdx )
{
    SetMappedUChar( gIdx, 2 );
}

// **************************
//
void                NOutputState::SetBIdx                       ( unsigned char bIdx )
{
    SetMappedUChar( bIdx, 4 );
}

// **************************
//
void                NOutputState::SetAIdx                       ( unsigned char aIdx )
{
    SetMappedUChar( aIdx, 6 );
}

// **************************
//
void                NOutputState::SetActiveRenderChannelType    ( RenderChannelType activeRenderChannel )
{
    m_activeRenderChannel = activeRenderChannel;
}

// **************************
//
void                NOutputState::SetMaskState                  ( bool rChannelEnabled, bool gChannelEnabled, bool bChannelEnabled, bool aChannelEnabled )
{
    SetRChannelState( rChannelEnabled );
    SetGChannelState( gChannelEnabled );
    SetBChannelState( bChannelEnabled );
    SetAChannelState( aChannelEnabled );
}

// **************************
//
void                NOutputState::SetRChannelState              ( bool isEnabled )
{
    m_rMask = isEnabled;
}

// **************************
//
void                NOutputState::SetGChannelState              ( bool isEnabled )
{
    m_gMask = isEnabled;
}

// **************************
//
void                NOutputState::SetBChannelState              ( bool isEnabled )
{
    m_bMask = isEnabled;
}

// **************************
//
void                NOutputState::SetAChannelState              ( bool isEnabled )
{
    m_aMask = isEnabled;
}

// **************************
//
unsigned int        NOutputState::GetWidth                      () const
{
    return m_width;
}

// **************************
//
unsigned int        NOutputState::GetHeight                     () const
{
    return m_height;
}

// **************************
//
unsigned int        NOutputState::GetChannelMapping             () const
{
    return m_channelMapping;
}

// **************************
//
unsigned int        NOutputState::GetRIdx                       () const
{
    return GetMappedUChar( 0 );
}

// **************************
//
unsigned int        NOutputState::GetGIdx                       () const
{
    return GetMappedUChar( 2 );
}

// **************************
//
unsigned int        NOutputState::GetBIdx                       () const
{
    return GetMappedUChar( 4 );
}

// **************************
//
unsigned int        NOutputState::GetAIdx                       () const
{
    return GetMappedUChar( 6 );
}

// **************************
//
RenderChannelType   NOutputState::GetActiveRenderChannelType    () const
{
    return m_activeRenderChannel;
}

// **************************
//
void                NOutputState::SetMappedUChar                ( unsigned int val, unsigned int numBits )
{
    assert( val < 4 );

    m_channelMapping = ( m_channelMapping & ~( 0xFF << numBits ) ) | ( val & 0x3 ) << numBits;
}

// **************************
//
unsigned int        NOutputState::GetMappedUChar                ( unsigned int numBits ) const
{
    return ( m_channelMapping & ( 0xFF << numBits ) ) >> numBits;
}

// **************************
//
glm::vec4           NOutputState::GetChannelMask                () const
{
    return glm::vec4( m_rMask ? 1.0f : 0.0f,
                      m_gMask ? 1.0f : 0.0f,
                      m_bMask ? 1.0f : 0.0f,
                      m_aMask ? 1.0f : 0.0f );
}

// **************************
//
bool                NOutputState::GetRChannelState          () const
{
    return m_rMask;
}

// **************************
//
bool                NOutputState::GetGChannelState          () const
{
    return m_gMask;
}

// **************************
//
bool                NOutputState::GetBChannelState          () const
{
    return m_bMask;
}

// **************************
//
bool                NOutputState::GetAChannelState          () const
{
    return m_aMask;
}

// **************************
//
bool                NOutputState::RepresentsDefaultTexture  () const
{
    static unsigned int noMapping = ( 0 | 1 << 2 | 2 << 4 | 3 << 6 );
    auto fullMask = m_rMask && m_bMask && m_gMask && m_aMask;

    return fullMask && m_channelMapping == noMapping;
}

} //nrl
} //bv
