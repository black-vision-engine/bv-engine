#include "stdafx.h"

#include "PdrSource.h"




#include "Memory/MemoryLeaks.h"



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

std::string getALErrorString( ALenum err )
{
	switch( err )
	{
	case AL_NO_ERROR:       return std::string( "AL_NO_ERROR - (No error)." ); break;
	case AL_INVALID_NAME:       return std::string( "AL_INVALID_NAME - Invalid Name paramater passed to AL call." ); break;
	case AL_INVALID_ENUM:       return std::string( "AL_INVALID_ENUM - Invalid parameter passed to AL call." ); break;
	case AL_INVALID_VALUE:      return std::string( "AL_INVALID_VALUE - Invalid enum parameter value." ); break;
	case AL_INVALID_OPERATION:  return std::string( "AL_INVALID_OPERATION" ); break;
	case AL_OUT_OF_MEMORY:      return std::string( "AL_OUT_OF_MEMORY" ); break;
	default:            return std::string( "AL Unknown Error." ); break;
	};
}

// *******************************
//
void	PdrSource::SetGain		( float gain )
{
	BVAL::bvalSourcef ( m_sourceHandle, AL_GAIN, gain );
}

} // audio
} // bv