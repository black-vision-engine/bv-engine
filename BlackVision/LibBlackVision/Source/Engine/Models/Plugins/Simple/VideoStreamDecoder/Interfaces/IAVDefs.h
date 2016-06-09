#pragma once

#include "Memory/MemoryChunk.h"
#include "Engine/Types/Enums.h"
#include "CoreDef.h"


namespace bv {

struct VideoMediaData
{
	UInt32			frameIdx;
	MemoryChunkPtr	frameData;

	VideoMediaData()
		: frameIdx( 0 )
		, frameData( nullptr )
	{
    }
};

struct AudioMediaData
{
	UInt32			frameIdx;
	MemoryChunkPtr	frameData;
    Int32           sampleRate;
    AudioFormat     format;

	AudioMediaData()
		: frameIdx( 0 )
		, frameData( nullptr )
		, sampleRate( 0 )
		, format( AudioFormat::MONO8 )
	{
    }
};

} //bv
