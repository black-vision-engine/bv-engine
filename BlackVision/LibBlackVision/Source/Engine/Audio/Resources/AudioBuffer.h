#pragma once

#include "Memory/MemoryChunk.h"
#include "Engine/Types/Enums.h"
#include "CoreDEF.h"


namespace bv { namespace audio {

class AudioBuffer;
DEFINE_PTR_TYPE(AudioBuffer)
DEFINE_CONST_PTR_TYPE(AudioBuffer)

class AudioBuffer
{
private:
    
    MemoryChunkConstPtr	m_data;
    Int32               m_frequency;
    AudioFormat         m_format;

public:

                    AudioBuffer	        ( MemoryChunkConstPtr data, Int32 frequency, AudioFormat format );
    virtual         ~AudioBuffer	    ();

    SizeType		GetSize	            () const;
    Int32		    GetFrequency	    () const;
    AudioFormat		GetFormat	        () const;
    Int32		    GetChannels	        () const;
    Int32		    GetChannelBitDepth  () const;

    MemoryChunkConstPtr GetData			() const;
    const char *	    GetRawData		() const;

    static AudioBufferPtr   Create	    ( MemoryChunkConstPtr data, Int32 frequency, AudioFormat format );

};

} // audio
} // bv
