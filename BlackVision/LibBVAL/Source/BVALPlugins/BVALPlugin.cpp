#include "BVALPlugin.h"

#include "al.h"
#include "alc.h"


namespace bv {

// *****************************
//
            BVALPlugin::~BVALPlugin						    ()
{
    QuitBVAL();
}

// *****************************
//
bool        BVALPlugin::InitializeBVAL						()
{
    auto success = false; 

    auto device = alcOpenDevice( nullptr );
    
    if( device )
    {
        auto ctx = alcCreateContext( device, nullptr );
        if( ctx )
        {
            alcMakeContextCurrent( ctx );
            success = true;
        }
    }

    alGetError();

    return success;
}

// *****************************
//
void        BVALPlugin::QuitBVAL						    ()
{
    auto ctx = alcGetCurrentContext();
    if( ctx )
    {
        auto device = alcGetContextsDevice( ctx );
        alcMakeContextCurrent( nullptr );
        alcDestroyContext( ctx );

        if( device )
        {
            alcCloseDevice( device );
        }
    }
}

// *****************************
//
ALenum      BVALPlugin::GetError						    ()
{
    return alGetError();
}

// *****************************
//
void        BVALPlugin::GenBuffers						    ( ALsizei n, ALuint * buffers )
{
    alGenBuffers( n, buffers );
}

// *****************************
//
void        BVALPlugin::DeleteBuffers						( ALsizei n, ALuint * buffers )
{
    alDeleteBuffers( n, buffers );
}

// *****************************
//
void        BVALPlugin::GenSources						    ( ALsizei n, ALuint * sources )
{
    alGenSources( n, sources );
}

// *****************************
//
void        BVALPlugin::DeleteSources						( ALsizei n, ALuint * sources )
{
    alDeleteSources( n, sources );
}

// *****************************
//
void        BVALPlugin::BufferData						    ( ALuint buffer, ALenum format, const ALvoid *data, ALsizei size, ALsizei frequency )
{
    alBufferData( buffer, format, data, size, frequency );
}

// *****************************
//
void        BVALPlugin::Bufferf						        ( ALuint buffer, ALenum param, ALfloat value )
{
    alBufferf( buffer, param, value );
}

// *****************************
//
void        BVALPlugin::Buffer3f						    ( ALuint buffer, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 )
{
    alBuffer3f( buffer, param, v1, v2, v3 );
}

// *****************************
//
void        BVALPlugin::Bufferfv						    ( ALuint buffer, ALenum param, ALfloat * values )
{
    alBufferfv( buffer, param, values );
}

// *****************************
//
void        BVALPlugin::Bufferi						        ( ALuint buffer, ALenum param, ALint value )
{
    alBufferi( buffer, param, value );
}

// *****************************
//
void        BVALPlugin::Buffer3i						    ( ALuint buffer, ALenum param, ALint v1, ALint v2, ALint v3 )
{
    alBuffer3i( buffer, param, v1, v2, v3 );
}

// *****************************
//
void        BVALPlugin::Bufferiv						    ( ALuint buffer, ALenum param, ALint * values )
{
    alBufferiv( buffer, param, values );
}

// *****************************
//
void        BVALPlugin::Sourcef						        ( ALuint source, ALenum param, ALfloat value )
{
    alSourcef( source, param, value );
}

// *****************************
//
void        BVALPlugin::Source3f						    ( ALuint source, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 )
{
    alSource3f( source, param, v1, v2, v3 );
}

// *****************************
//
void        BVALPlugin::Sourcefv						    ( ALuint source, ALenum param, ALfloat * values )
{
    alSourcefv( source, param, values );
}

// *****************************
//
void        BVALPlugin::Sourcei						        ( ALuint source, ALenum param, ALint value )
{
    alSourcei( source, param, value );
}

// *****************************
//
void        BVALPlugin::Source3i						    ( ALuint source, ALenum param, ALint v1, ALint v2, ALint v3 )
{
    alSource3i( source, param, v1, v2, v3 );
}

// *****************************
//
void        BVALPlugin::Sourceiv						    ( ALuint source, ALenum param, ALint * values )
{
    alSourceiv( source, param, values );
}

// *****************************
//
void        BVALPlugin::GetSourcef						    ( ALuint source, ALenum param, ALfloat * value )
{
    alGetSourcef( source, param, value );
}

// *****************************
//
void        BVALPlugin::GetSource3f						    ( ALuint source, ALenum param, ALfloat * v1, ALfloat * v2, ALfloat * v3 )
{
    alGetSource3f( source, param, v1, v2, v3 );
}

// *****************************
//
void        BVALPlugin::GetSourcefv						    ( ALuint source, ALenum param, ALfloat * values )
{
    alGetSourcefv( source, param, values );
}

// *****************************
//
void        BVALPlugin::GetSourcei						    ( ALuint source, ALenum param, ALint * value )
{
    alGetSourcei( source, param, value );
}

// *****************************
//
void        BVALPlugin::GetSource3i						    ( ALuint source, ALenum param, ALint * v1, ALint * v2, ALint * v3 )
{
    alGetSource3i( source, param, v1, v2, v3 );
}

// *****************************
//
void        BVALPlugin::GetSourceiv						    ( ALuint source, ALenum param, ALint * values )
{
    alGetSourceiv( source, param, values );
}

// *****************************
//
void        BVALPlugin::SourcePlay						    ( ALuint source )
{
    alSourcePlay( source );
}

// *****************************
//
void        BVALPlugin::SourcePause						    ( ALuint source )
{
    alSourcePause( source );
}

// *****************************
//
void        BVALPlugin::SourceStop						    ( ALuint source )
{
    alSourceStop( source );
}

// *****************************
//
void        BVALPlugin::SourceRewind						( ALuint source )
{
    alSourceRewind( source );
}

// *****************************
//
void        BVALPlugin::SourceQueueBuffers					( ALuint source, ALsizei n, ALuint * buffers )
{
    alSourceQueueBuffers( source, n, buffers );
}

// *****************************
//
void        BVALPlugin::SourceUnqueueBuffers				( ALuint source, ALsizei n, ALuint * buffers )
{
    alSourceUnqueueBuffers( source, n, buffers );
}

// *****************************
//
void        BVALPlugin::Listenerf						    ( ALenum param, ALfloat value )
{
    alListenerf( param, value );
}

// *****************************
//
void        BVALPlugin::Listener3f						    ( ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 )
{
    alListener3f( param, v1, v2, v3 );
}

// *****************************
//
void        BVALPlugin::Listenerfv						    ( ALenum param, ALfloat * values )
{
    alListenerfv( param, values );
}

// *****************************
//
void        BVALPlugin::Listeneri						    ( ALenum param, ALint value )
{
    alListeneri( param, value );
}

// *****************************
//
void        BVALPlugin::Listener3i						    ( ALenum param, ALint v1, ALint v2, ALint v3 )
{
    alListener3i( param, v1, v2, v3 );
}

// *****************************
//
void        BVALPlugin::Listeneriv						    ( ALenum param, ALint * values )
{
    alListeneriv( param, values );
}

} //bv