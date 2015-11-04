#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"
#include "Memory/MemoryChunk.h"

namespace bv {

class IAnimationDescriptor : public ITextureParams
{
public:

    virtual unsigned int            NumTextures         () const = 0;
    virtual MemoryChunkConstPtr     GetBits             ( unsigned int txNum ) const = 0;

    virtual bool                    BitsChanged         () const = 0;
    virtual bool                    BitsChanged         ( unsigned int frameNum ) const = 0;
    virtual void                    ResetBitsChanged    ( unsigned int frameNum ) const = 0;

    virtual unsigned int            CurrentFrame        () const = 0;
    virtual unsigned int            PreviousFrame       () const = 0;

    virtual ~IAnimationDescriptor() {}

};

DEFINE_PTR_TYPE(IAnimationDescriptor)
DEFINE_CONST_PTR_TYPE(IAnimationDescriptor)

} //bv
