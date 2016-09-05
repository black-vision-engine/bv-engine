#include "BVAL.h"
#include "BVALPlugins\BVALPluginFactory.h"


namespace bv {

namespace {
    
BVALPlugin &	bval = BVALPluginFactory::BVAL	();

} // anonymous


// *****************************
// FIXME: this initialization should check much more caps and return an error code instead of a simple bool
bool        BVAL::InitializeBVAL                ()
{
    return bval.InitializeBVAL();
}

// *****************************
//
ALenum      BVAL::bvalGetError                  ()
{
    return bval.GetError();
}

// *****************************
//
void        BVAL::bvalGenBuffers                ( ALsizei n, ALuint * buffers )
{
    bval.GenBuffers( n, buffers );
}

// *****************************
//
void        BVAL::bvalDeleteBuffers             ( ALsizei n, ALuint * buffers )
{
    bval.DeleteBuffers( n, buffers );
}

// *****************************
//
void        BVAL::bvalGenSources                ( ALsizei n, ALuint * sources )
{
    bval.GenSources( n, sources );
}

// *****************************
//
void        BVAL::bvalDeleteSources             ( ALsizei n, ALuint * sources )
{
    bval.DeleteSources( n, sources );
}

// *****************************
//
void        BVAL::bvalBufferData                ( ALuint buffer, ALenum format, const ALvoid * data, ALsizei size, ALsizei frequency )
{
    bval.BufferData( buffer, format, data, size, frequency );
}

// *****************************
//
void        BVAL::bvalBufferf                   ( ALuint buffer, ALenum param, ALfloat value )
{
    bval.Bufferf( buffer, param, value );
}

// *****************************
//
void        BVAL::bvalBuffer3f                  ( ALuint buffer, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 )
{
    bval.Buffer3f( buffer, param, v1, v2, v3 );
}

// *****************************
//
void        BVAL::bvalBufferfv                  ( ALuint buffer, ALenum param, ALfloat * values )
{
    bval.Bufferfv( buffer, param, values );
}

// *****************************
//
void        BVAL::bvalBufferi                   ( ALuint buffer, ALenum param, ALint value )
{
    bval.Bufferi( buffer, param, value );
}

// *****************************
//
void        BVAL::bvalBuffer3i                  ( ALuint buffer, ALenum param, ALint v1, ALint v2, ALint v3 )
{
    bval.Buffer3i( buffer, param, v1, v2, v3 );
}

// *****************************
//
void        BVAL::bvalBufferiv                  ( ALuint buffer, ALenum param, ALint * values )
{
    bval.Bufferiv( buffer, param, values );
}

// *****************************
//
void        BVAL::bvalSourcef                   ( ALuint source, ALenum param, ALfloat value )
{
    bval.Sourcef( source, param, value );
}

// *****************************
//
void        BVAL::bvalSource3f                  ( ALuint source, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 )
{
    bval.Source3f( source, param, v1, v2, v3 );
}

// *****************************
//
void        BVAL::bvalSourcefv                  ( ALuint source, ALenum param, ALfloat * values )
{
    bval.Sourcefv( source, param, values );
}

// *****************************
//
void        BVAL::bvalSourcei                   ( ALuint source, ALenum param, ALint value )
{
    bval.Sourcei( source, param, value );
}

// *****************************
//
void        BVAL::bvalSource3i                  ( ALuint source, ALenum param, ALint v1, ALint v2, ALint v3 )
{
    bval.Source3i( source, param, v1, v2, v3 );
}

// *****************************
//
void        BVAL::bvalSourceiv                  ( ALuint source, ALenum param, ALint * values )
{
    bval.Sourceiv( source, param, values );
}


// *****************************
//
void        BVAL::bvalGetSourcef                ( ALuint source, ALenum param, ALfloat * value )
{
    bval.GetSourcef( source, param, value );
}

// *****************************
//
void        BVAL::bvalGetSource3f                   ( ALuint source, ALenum param, ALfloat * v1, ALfloat * v2, ALfloat * v3 )
{
    bval.GetSource3f( source, param, v1, v2, v3 );
}

// *****************************
//
void        BVAL::bvalGetSourcefv                   ( ALuint source, ALenum param, ALfloat * values )
{
    bval.GetSourcefv( source, param, values );
}

// *****************************
//
void        BVAL::bvalGetSourcei                    ( ALuint source, ALenum param, ALint * value )
{
    bval.GetSourcei( source, param, value );
}

// *****************************
//
void        BVAL::bvalGetSource3i                   ( ALuint source, ALenum param, ALint * v1, ALint * v2, ALint * v3 )
{
    bval.GetSource3i( source, param, v1, v2, v3 );
}

// *****************************
//
void        BVAL::bvalGetSourceiv                   ( ALuint source, ALenum param, ALint * values )
{
    bval.GetSourceiv( source, param, values );
}

// *****************************
//
void        BVAL::bvalSourcePlay                ( ALuint source )
{
    bval.SourcePlay( source );
}

// *****************************
//
void        BVAL::bvalSourcePause               ( ALuint source )
{
    bval.SourcePause( source );
}

// *****************************
//
void        BVAL::bvalSourceStop                ( ALuint source )
{
    bval.SourceStop( source );
}

// *****************************
//
void        BVAL::bvalSourceRewind              ( ALuint source )
{
    bval.SourceRewind( source );
}

// *****************************
//
void        BVAL::bvalSourceQueueBuffers        ( ALuint source, ALsizei n, ALuint * buffers  )
{
    bval.SourceQueueBuffers( source, n, buffers );
}

// *****************************
//
void        BVAL::bvalSourceUnqueueBuffers      ( ALuint source, ALsizei n, ALuint * buffers  )
{
    bval.SourceUnqueueBuffers( source, n, buffers );
}

// *****************************
//
void        BVAL::bvalListenerf                 ( ALenum param, ALfloat value )
{
    bval.Listenerf( param, value );
}

// *****************************
//
void        BVAL::bvalListener3f                ( ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 )
{
    bval.Listener3f( param, v1, v2, v3 );
}

// *****************************
//
void        BVAL::bvalListenerfv                ( ALenum param, ALfloat * values )
{
    bval.Listenerfv( param, values );
}

// *****************************
//
void        BVAL::bvalListeneri                 ( ALenum param, ALint value )
{
    bval.Listeneri( param, value );
}

// *****************************
//
void        BVAL::bvalListener3i                ( ALenum param, ALint v1, ALint v2, ALint v3 )
{
    bval.Listener3i( param, v1, v2, v3 );
}

// *****************************
//
void        BVAL::bvalListeneriv                ( ALenum param, ALint * values )
{
    bval.Listeneriv( param, values );
}

} // bv