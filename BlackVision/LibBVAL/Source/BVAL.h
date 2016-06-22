#pragma once

#include "BVALDefs.h"


namespace bv {

class BVAL
{
public:

    static bool                 InitializeBVAL              ();

    static ALenum               bvalGetError					();

    static void                 bvalGenBuffers					( ALsizei n, ALuint * buffers );
    static void                 bvalDeleteBuffers				( ALsizei n, ALuint * buffers );
    
    static void                 bvalGenSources					( ALsizei n, ALuint * sources );
    static void                 bvalDeleteSources				( ALsizei n, ALuint * sources );

    static void                 bvalBufferData					( ALuint buffer, ALenum format, const ALvoid * data, ALsizei size, ALsizei frequency );
    
    static void                 bvalBufferf					    ( ALuint buffer, ALenum param, ALfloat value );
    static void                 bvalBuffer3f					( ALuint buffer, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 );
    static void                 bvalBufferfv					( ALuint buffer, ALenum param, ALfloat * values );
    static void                 bvalBufferi					    ( ALuint buffer, ALenum param, ALint value );
    static void                 bvalBuffer3i					( ALuint buffer, ALenum param, ALint v1, ALint v2, ALint v3 );
    static void                 bvalBufferiv					( ALuint buffer, ALenum param, ALint * values );

    static void                 bvalSourcef					    ( ALuint source, ALenum param, ALfloat value );
    static void                 bvalSource3f					( ALuint source, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 );
    static void                 bvalSourcefv					( ALuint source, ALenum param, ALfloat * values );
    static void                 bvalSourcei					    ( ALuint source, ALenum param, ALint value );
    static void                 bvalSource3i					( ALuint source, ALenum param, ALint v1, ALint v2, ALint v3 );
    static void                 bvalSourceiv					( ALuint source, ALenum param, ALint * values );

    static void                 bvalGetSourcef					( ALuint source, ALenum param, ALfloat * value );
    static void                 bvalGetSource3f					( ALuint source, ALenum param, ALfloat * v1, ALfloat * v2, ALfloat * v3 );
    static void                 bvalGetSourcefv					( ALuint source, ALenum param, ALfloat * values );
    static void                 bvalGetSourcei					( ALuint source, ALenum param, ALint * value );
    static void                 bvalGetSource3i					( ALuint source, ALenum param, ALint * v1, ALint * v2, ALint * v3 );
    static void                 bvalGetSourceiv					( ALuint source, ALenum param, ALint * values );

    static void                 bvalSourcePlay                  ( ALuint source );
    static void                 bvalSourcePause                 ( ALuint source );
    static void                 bvalSourceStop                  ( ALuint source );
    static void                 bvalSourceRewind                ( ALuint source );

    static void                 bvalSourceQueueBuffers          ( ALuint source, ALsizei n, ALuint * buffers );
    static void                 bvalSourceUnqueueBuffers        ( ALuint source, ALsizei n, ALuint * buffers );

    static void                 bvalListenerf                   ( ALenum param, ALfloat value );
    static void                 bvalListener3f                  ( ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 );
    static void                 bvalListenerfv                  ( ALenum param, ALfloat * values );
    static void                 bvalListeneri                   ( ALenum param, ALint value );
    static void                 bvalListener3i                  ( ALenum param, ALint v1, ALint v2, ALint v3 );
    static void                 bvalListeneriv                  ( ALenum param, ALint * values );

};

} //bv
