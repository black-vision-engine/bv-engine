#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"
#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Memory/MemoryChunk.h"

namespace bv {

class ITextureDescriptor : public ITextureParams
{
public:

    typedef uintptr_t   uid_t;

public:

    virtual uid_t                   GetUID              () const = 0;

    virtual MemoryChunkConstPtr     GetBits             () const = 0;
    virtual bool                    BitsChanged         () const = 0;
    virtual void                    ResetBitsChanged    () const = 0;
    virtual DataBuffer::Semantic    GetSemantic         () const = 0;

    virtual ~ITextureDescriptor() {}

};

DEFINE_PTR_TYPE(ITextureDescriptor)
DEFINE_CONST_PTR_TYPE(ITextureDescriptor)

} //bv
