#pragma once

#include "Engine/Types/Enums.h"
#include "CoreDEF.h"


namespace bv { namespace audio {

class AudioUtils
{
public:

    static Int32        ChannelsCount   ( AudioFormat format );
    static Int32        ChannelBitDepth ( AudioFormat format );

};

} //audio
} //bv
