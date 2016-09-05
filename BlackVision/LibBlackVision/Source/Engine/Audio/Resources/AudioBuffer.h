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
    
    SizeType            m_size;
    Int32               m_frequency;
    AudioFormat         m_format;
    char *		        m_data;

public:

                    AudioBuffer	        ( SizeType size, Int32 frequency, AudioFormat format );
    virtual         ~AudioBuffer	    ();

    SizeType		GetSize	            () const;
    Int32		    GetFrequency	    () const;
    AudioFormat		GetFormat	        () const;

    const char *	Data			    () const;
    void            WriteToBuffer       ( const char * src, SizeType size );

    static AudioBufferPtr   Create	    ( const std::vector< MemoryChunkConstPtr > & data, Int32 frequency, AudioFormat format );
    static AudioBufferPtr   Create	    ( const char * data, SizeType size, Int32 frequency, AudioFormat format );

};

} // audio
} // bv
