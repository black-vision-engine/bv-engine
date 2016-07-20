#pragma once

#include "DataTypes/Queue.h"
#include "Engine/Audio/Resources/AudioBuffer.h"
#include "Memory/MemoryChunk.h"


namespace bv { namespace audio {


class AudioEntity
{
private:

    typedef std::vector< audio::AudioBufferConstPtr > AudioBufferVec;

protected:

    Queue< audio::AudioBufferConstPtr > m_audioData;

    Int32                               m_frequency;
    AudioFormat                         m_format;

	UInt32							    m_updateID;

public:

                                AudioEntity     ( Int32 frequency, AudioFormat format );
    virtual                     ~AudioEntity    ();

    void                        Reinitialize    ( Int32 frequency, AudioFormat format );

    Int32                       GetFrequency    () const;
    AudioFormat                 GetFormat       () const;

    bool                        IsEmpty         () const;

    void                        PushData        ( const std::vector< MemoryChunkConstPtr > & data );
    AudioBufferVec              PopData         ();

    UInt32                      GetUpdateID     () const;

};

} //audio
} //bv