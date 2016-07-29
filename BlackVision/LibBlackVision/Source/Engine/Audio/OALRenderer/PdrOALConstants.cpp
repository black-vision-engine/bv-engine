#include "stdafx.h"

#include "PdrOALConstants.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace audio {

namespace
{
    ALenum AudioFormatAL[] =
    {
        AL_FORMAT_MONO8,        // MONO8,
        AL_FORMAT_STEREO8,      // STEREO8,
        AL_FORMAT_MONO16,       // MONO16,
        AL_FORMAT_STEREO16      // STEREO16,
    };

} //anonymous

// ****************************
//
ALenum      OALConstantsMapper::ALConstant     ( AudioFormat format )
{
    return AudioFormatAL[ ( Int32 )format ]; 
}

} //audio
} //bv
