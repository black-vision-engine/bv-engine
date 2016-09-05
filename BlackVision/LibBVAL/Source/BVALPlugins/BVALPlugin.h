#pragma once

#include <string>
#include <cstddef>

#include "BVALDefs.h"


namespace bv {

class BVALPlugin
{
public:
    
    virtual                     ~BVALPlugin                 ();

    virtual bool				InitializeBVAL				();
    virtual void				QuitBVAL			        ();

    virtual ALenum              GetError					();

    virtual void                GenBuffers					( ALsizei n, ALuint * buffers );
    virtual void                DeleteBuffers				( ALsizei n, ALuint * buffers );
    
    virtual void                GenSources					( ALsizei n, ALuint * sources );
    virtual void                DeleteSources				( ALsizei n, ALuint * sources );

    virtual void                BufferData					( ALuint buffer, ALenum format, const ALvoid * data, ALsizei size, ALsizei frequency );
    
    virtual void                Bufferf					    ( ALuint buffer, ALenum param, ALfloat value );
    virtual void                Buffer3f					( ALuint buffer, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 );
    virtual void                Bufferfv					( ALuint buffer, ALenum param, ALfloat * values );
    virtual void                Bufferi					    ( ALuint buffer, ALenum param, ALint value );
    virtual void                Buffer3i					( ALuint buffer, ALenum param, ALint v1, ALint v2, ALint v3 );
    virtual void                Bufferiv					( ALuint buffer, ALenum param, ALint * values );

    virtual void                Sourcef					    ( ALuint source, ALenum param, ALfloat value );
    virtual void                Source3f					( ALuint source, ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 );
    virtual void                Sourcefv					( ALuint source, ALenum param, ALfloat * values );
    virtual void                Sourcei					    ( ALuint source, ALenum param, ALint value );
    virtual void                Source3i					( ALuint source, ALenum param, ALint v1, ALint v2, ALint v3 );
    virtual void                Sourceiv					( ALuint source, ALenum param, ALint * values );

    virtual void                GetSourcef					( ALuint source, ALenum param, ALfloat * value );
    virtual void                GetSource3f					( ALuint source, ALenum param, ALfloat * v1, ALfloat * v2, ALfloat * v3 );
    virtual void                GetSourcefv					( ALuint source, ALenum param, ALfloat * values );
    virtual void                GetSourcei					( ALuint source, ALenum param, ALint * value );
    virtual void                GetSource3i					( ALuint source, ALenum param, ALint * v1, ALint * v2, ALint * v3 );
    virtual void                GetSourceiv					( ALuint source, ALenum param, ALint * values );

    virtual void                SourcePlay                  ( ALuint source );
    virtual void                SourcePause                 ( ALuint source );
    virtual void                SourceStop                  ( ALuint source );
    virtual void                SourceRewind                ( ALuint source );

    virtual void                SourceQueueBuffers          ( ALuint source, ALsizei n, ALuint * buffers );
    virtual void                SourceUnqueueBuffers        ( ALuint source, ALsizei n, ALuint * buffers );
    
    virtual void                Listenerf                   ( ALenum param, ALfloat value );
    virtual void                Listener3f                  ( ALenum param, ALfloat v1, ALfloat v2, ALfloat v3 );
    virtual void                Listenerfv                  ( ALenum param, ALfloat * values );
    virtual void                Listeneri                   ( ALenum param, ALint value );
    virtual void                Listener3i                  ( ALenum param, ALint v1, ALint v2, ALint v3 );
    virtual void                Listeneriv                  ( ALenum param, ALint * values );

};

} //bv
