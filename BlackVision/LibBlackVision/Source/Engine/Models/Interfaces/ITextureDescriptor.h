#pragma once

#include "Engine/Models/Interfaces/ITextureParams.h"
#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Memory/MemoryChunk.h"

namespace bv
{



// ***********************
//
class ITextureDescriptor : public ITextureParams
{
public:

    typedef uintptr_t   uid_t;

    enum class Type
    {
        Memory,     ///< Descriptor contain texture data in memory
        GPU         ///< Descriptor references texture residing in GPU.
    };

public:

    virtual Type                    GetTextureType      () const { return Type::Memory; }
    virtual uid_t                   GetUID              () const = 0;

	virtual UInt32				    GetNumLevels        () const = 0;
    virtual MemoryChunkConstPtr     GetBits             ( UInt32 level ) const = 0;
	virtual MemoryChunkVector		GetBits             () const = 0;
    virtual DataBuffer::Semantic    GetSemantic         () const = 0;
	
	virtual UInt64                  GetUpdateID         () const = 0;

    virtual ~ITextureDescriptor() {}

};

DEFINE_PTR_TYPE(ITextureDescriptor)
DEFINE_CONST_PTR_TYPE(ITextureDescriptor)

} //bv
