#pragma once

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"

namespace bv { namespace model
{

class RendererContext;

class IAudioChannel
{
public:

    virtual                             ~IAudioChannel      () {};
    
    virtual void                        PushPacket          ( MemoryChunkPtr packet ) = 0;
    virtual MemoryChunkPtr              PopPacket           () = 0;

    virtual Int32                       GetFrequency        () const = 0;
    virtual AudioFormat                 GetFormat           () const = 0;

};

DEFINE_PTR_TYPE(IAudioChannel)
DEFINE_CONST_PTR_TYPE(IAudioChannel)

} // model
} // bv
