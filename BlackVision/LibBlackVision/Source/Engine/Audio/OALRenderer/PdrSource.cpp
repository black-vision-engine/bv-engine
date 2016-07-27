#include "stdafx.h"

#include "PdrSource.h"


namespace bv { namespace audio {


// *******************************
//
PdrSource::PdrSource            ()
{
    BVAL::bvalGenSources( 1, &m_sourceHandle );
    BVAL::bvalSourcef( m_sourceHandle, AL_LOOPING, false );
}

// *******************************
//
PdrSource::~PdrSource           ()
{
    BVAL::bvalDeleteSources( 1, &m_sourceHandle );
}

// *******************************
//
ALuint  PdrSource::GetHandle    () const
{
    return m_sourceHandle;
}

// *******************************
//
void    PdrSource::Play         ()
{
    if( !IsPlaying() )
    {
        BVAL::bvalSourcePlay( m_sourceHandle );
    }
}

// *******************************
//
void    PdrSource::Pause        ()
{
    if( !IsPaused() )
    {
        BVAL::bvalSourcePause( m_sourceHandle );
    }
}

// *******************************
//
void    PdrSource::Stop         ()
{
    if( !IsStopped() )
    {
        BVAL::bvalSourceStop( m_sourceHandle );
    }
}

// *******************************
//
void    PdrSource::Rewind       ()
{
    BVAL::bvalSourceRewind( m_sourceHandle );
}

// *******************************
//
bool    PdrSource::IsPlaying    () const
{
    ALint state = 0;
    BVAL::bvalGetSourcei( m_sourceHandle, AL_SOURCE_STATE, &state );
    return ( state == AL_PLAYING );
}

// *******************************
//
bool    PdrSource::IsPaused     () const
{
    ALint state = 0;
    BVAL::bvalGetSourcei( m_sourceHandle, AL_SOURCE_STATE, &state );
    return ( state == AL_PLAYING );
}

// *******************************
//
bool    PdrSource::IsStopped    () const
{
    ALint state = 0;
    BVAL::bvalGetSourcei( m_sourceHandle, AL_SOURCE_STATE, &state );
    return ( state == AL_STOPPED );
}

} // audio
} // bv