#pragma once

#include "Engine/Types/Enums.h"
#include "BVAL.h"


namespace bv { namespace audio {

class OALConstantsMapper
{
public:

    static ALenum       ALConstant      ( AudioFormat format );

};

} //audio
} //bv
