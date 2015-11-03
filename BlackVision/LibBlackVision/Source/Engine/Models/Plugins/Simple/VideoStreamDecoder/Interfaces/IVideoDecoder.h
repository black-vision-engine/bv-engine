#pragma once

#include "Memory/MemoryChunk.h"
#include "CoreDef.h"

namespace bv
{

class IVideoDecoder
{
public:
	virtual void					StartDecoding			() = 0;

	virtual MemoryChunkConstPtr		GetCurrentFrameData		( UInt32 & outFrameId ) const = 0;
	virtual bool					GetNextFrameData		() = 0;

	virtual UInt32					GetWidth				() const = 0;
	virtual UInt32					GetHeight				() const = 0;
	virtual UInt32					GetFrameRate			() const = 0;
};

DEFINE_PTR_TYPE( IVideoDecoder )
DEFINE_CONST_PTR_TYPE( IVideoDecoder )

} //bv
