#pragma once

#include "Memory/MemoryChunk.h"
#include "Engine/Types/Enums.h"
#include "CoreDef.h"


namespace bv {

struct AVMediaData
{
	UInt32			frameIdx;
	MemoryChunkPtr	frameData;
    SizeType        nbSamples;

	AVMediaData()
		: frameIdx( 0 )
		, frameData( nullptr )
        , nbSamples( 0 )
	{
    }
};


} //bv
