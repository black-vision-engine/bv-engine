#include "stdafx.h"

#include "OutputState.h"


namespace bv { 

// **************************
//
OutputState::OutputState                                      ( unsigned int w, unsigned int h )
    : m_width( w )
    , m_height( h )
    , m_channelMapping( 0 )
{
    SetChannelMapping( 0, 1, 2, 3 );

    SetMaskState( true, true, true, true );
}

// **************************
//
void                OutputState::SetWidth                      ( unsigned int w )
{
    m_width = w;
}

// **************************
//
void                OutputState::SetHeight                     ( unsigned int h )
{
    m_height = h;
}

// **************************
//
void                OutputState::SetChannelMapping             ( unsigned int channelMapping )
{
    m_channelMapping = channelMapping;
}

// **************************
//
void                OutputState::SetChannelMapping             ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx )
{
    m_channelMapping = 0;

    SetRIdx( rIdx );
    SetGIdx( gIdx );
    SetBIdx( bIdx );
    SetAIdx( aIdx );
}

// **************************
//
void                OutputState::SetRIdx                       ( unsigned char rIdx )
{
    SetMappedUChar( rIdx, 0 );
}

// **************************
//
void                OutputState::SetGIdx                       ( unsigned char gIdx )
{
    SetMappedUChar( gIdx, 2 );
}

// **************************
//
void                OutputState::SetBIdx                       ( unsigned char bIdx )
{
    SetMappedUChar( bIdx, 4 );
}

// **************************
//
void                OutputState::SetAIdx                       ( unsigned char aIdx )
{
    SetMappedUChar( aIdx, 6 );
}

// **************************
//
void                OutputState::SetActiveRenderChannelType    ( RenderChannelType activeRenderChannel )
{
    m_activeRenderChannel = activeRenderChannel;
}

// **************************
//
void                OutputState::SetMaskState                  ( bool rChannelEnabled, bool gChannelEnabled, bool bChannelEnabled, bool aChannelEnabled )
{
    SetRChannelState( rChannelEnabled );
    SetGChannelState( gChannelEnabled );
    SetBChannelState( bChannelEnabled );
    SetAChannelState( aChannelEnabled );
}

// **************************
//
void                OutputState::SetRChannelState              ( bool isEnabled )
{
    m_rMask = isEnabled;
}

// **************************
//
void                OutputState::SetGChannelState              ( bool isEnabled )
{
    m_gMask = isEnabled;
}

// **************************
//
void                OutputState::SetBChannelState              ( bool isEnabled )
{
    m_bMask = isEnabled;
}

// **************************
//
void                OutputState::SetAChannelState              ( bool isEnabled )
{
    m_aMask = isEnabled;
}

// **************************
//
unsigned int        OutputState::GetWidth                      () const
{
    return m_width;
}

// **************************
//
unsigned int        OutputState::GetHeight                     () const
{
    return m_height;
}

// **************************
//
unsigned int        OutputState::GetChannelMapping             () const
{
    return m_channelMapping;
}

// **************************
//
unsigned int        OutputState::GetRIdx                       () const
{
    return GetMappedUChar( 0 );
}

// **************************
//
unsigned int        OutputState::GetGIdx                       () const
{
    return GetMappedUChar( 2 );
}

// **************************
//
unsigned int        OutputState::GetBIdx                       () const
{
    return GetMappedUChar( 4 );
}

// **************************
//
unsigned int        OutputState::GetAIdx                       () const
{
    return GetMappedUChar( 6 );
}

// **************************
//
RenderChannelType   OutputState::GetActiveRenderChannelType    () const
{
    return m_activeRenderChannel;
}

// **************************
//
void                OutputState::SetMappedUChar                ( unsigned int val, unsigned int numBits )
{
    assert( val < 4 );

    m_channelMapping = ( m_channelMapping & ~( 0xFF << numBits ) ) | ( val & 0x3 ) << numBits;
}

// **************************
//
unsigned int        OutputState::GetMappedUChar                ( unsigned int numBits ) const
{
    return ( m_channelMapping & ( 0xFF << numBits ) ) >> numBits;
}

// **************************
//
glm::vec4           OutputState::GetChannelMask                () const
{
    return glm::vec4( m_rMask ? 1.0f : 0.0f,
                      m_gMask ? 1.0f : 0.0f,
                      m_bMask ? 1.0f : 0.0f,
                      m_aMask ? 1.0f : 0.0f );
}

// **************************
//
bool                OutputState::GetRChannelState          () const
{
    return m_rMask;
}

// **************************
//
bool                OutputState::GetGChannelState          () const
{
    return m_gMask;
}

// **************************
//
bool                OutputState::GetBChannelState          () const
{
    return m_bMask;
}

// **************************
//
bool                OutputState::GetAChannelState          () const
{
    return m_aMask;
}

// **************************
//
bool                OutputState::RepresentsDefaultTexture  () const
{
    static unsigned int noMapping = ( 0 | 1 << 2 | 2 << 4 | 3 << 6 );
    auto fullMask = m_rMask && m_bMask && m_gMask && m_aMask;

    return fullMask && m_channelMapping == noMapping;
}


} //bv
