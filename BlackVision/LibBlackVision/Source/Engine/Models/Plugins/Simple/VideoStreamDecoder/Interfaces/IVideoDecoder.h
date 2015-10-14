#pragma once

#include "Memory/MemoryChunk.h"
#include "CoreDef.h"

namespace bv
{

class IVideoDecoder
{
public:
	virtual void					Start					() = 0;
	virtual void					Pause					() = 0;
	virtual void					Stop					() = 0;

	virtual MemoryChunkConstPtr		GetCurrentFrameData		( UInt64 & outFrameId ) const = 0;
	
	virtual bool					NextFrameDataReady		() = 0;

	virtual SizeType				GetFrameSize			() const = 0;

	virtual UInt32					GetWidth				() const = 0;
	virtual UInt32					GetHeight				() const = 0;
	virtual Float64					GetFrameRate			() const = 0;

	virtual void					Reset					() = 0;

	virtual bool					IsEOF					() const = 0;
};

DEFINE_PTR_TYPE( IVideoDecoder )
DEFINE_CONST_PTR_TYPE( IVideoDecoder )

} //bv
