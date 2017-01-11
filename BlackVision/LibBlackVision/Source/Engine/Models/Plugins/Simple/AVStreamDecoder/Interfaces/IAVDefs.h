#pragma once

#include "Memory/MemoryChunk.h"
#include "Engine/Types/Enums.h"
#include "CoreDef.h"


namespace bv {

struct AVMediaData
{
	UInt64			framePTS;   //miliseconds
	MemoryChunkPtr	frameData;
    SizeType        nbSamples;

	AVMediaData()
		: framePTS( 0 )
		, frameData( nullptr )
        , nbSamples( 0 )
	{
    }
};

} //bv
