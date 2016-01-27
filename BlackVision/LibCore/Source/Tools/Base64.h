#pragma once

#include "CoreDEF.h"
#include "Memory/MemoryChunk.h"

#include <string>

namespace bv
{

// ******************************
//
std::string         EncodeBase64    ( const MemoryChunkConstPtr data );

// ******************************
//
MemoryChunkConstPtr DecodeBase64    ( const std::string & enc );

} // bv