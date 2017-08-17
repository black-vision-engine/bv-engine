#pragma once

#include "DataTypes/Queue.h"
#include "Engine/Audio/Resources/AudioBuffer.h"
#include "Memory/MemoryChunk.h"


namespace bv { namespace audio {


class AudioEntity
{
protected:

    Queue< audio::AudioBufferConstPtr > m_audioData;

    Int32                               m_frequency;
    AudioFormat                         m_format;

	UInt32							    m_updateID;

    bool                                m_playing;

public:

                                AudioEntity     ( Int32 frequency, AudioFormat format );
    virtual                     ~AudioEntity    ();

    void                        Reinitialize    ( Int32 frequency, AudioFormat format );

    Int32                       GetFrequency    () const;
    AudioFormat                 GetFormat       () const;

    void                        Clear           ();
    bool                        IsEmpty         () const;

    void                        PushData        ( MemoryChunkConstPtr data );
    AudioBufferVec              PopData         ();
    bool                        IsEOF           () const;

    UInt32                      GetUpdateID     () const;

    bool                        IsPlaying       () const;
    void                        Play            ();
    void                        Stop            ();

};

} //audio
} //bv
