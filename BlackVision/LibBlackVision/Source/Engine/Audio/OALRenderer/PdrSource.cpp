#include "stdafx.h"

#include "PdrSource.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace audio {


// *******************************
//
PdrSource::PdrSource        ()
{
    BVAL::bvalGenSources( 1, &m_sourceHandle );
    BVAL::bvalSourcef( m_sourceHandle, AL_LOOPING, false );
}

// *******************************
//
PdrSource::~PdrSource       ()
{
    BVAL::bvalDeleteSources( 1, &m_sourceHandle );
}

// *******************************
//
ALuint  PdrSource::GetHandle      () const
{
    return m_sourceHandle;
}

// *******************************
//
void    PdrSource::Play     ()
{
    ALint state = 0;
    BVAL::bvalGetSourcei( m_sourceHandle, AL_SOURCE_STATE, &state );

    if( state != AL_PLAYING )
    {
        BVAL::bvalSourcePlay( m_sourceHandle );
    }
}

// *******************************
//
void    PdrSource::Pause     ()
{
    ALint state = 0;
    BVAL::bvalGetSourcei( m_sourceHandle, AL_SOURCE_STATE, &state );

    if( state != AL_PAUSED )
    {
        BVAL::bvalSourcePause( m_sourceHandle );
    }
}

// *******************************
//
void    PdrSource::Stop     ()
{
    ALint state = 0;
    BVAL::bvalGetSourcei( m_sourceHandle, AL_SOURCE_STATE, &state );

    if( state != AL_STOPPED )
    {
        BVAL::bvalSourceStop( m_sourceHandle );
    }
}

// *******************************
//
void    PdrSource::Rewind     ()
{
    BVAL::bvalSourceRewind( m_sourceHandle );
}

} // audio
} // bv