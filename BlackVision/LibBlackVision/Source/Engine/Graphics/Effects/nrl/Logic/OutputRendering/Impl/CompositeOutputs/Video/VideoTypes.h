#pragma once

#include <hash_map>
#include <vector>

#include "AVFrame.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputStaticData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannel.h"


namespace bv { 
    
using videocards::AVFramePtr;

namespace nrl {

typedef unsigned int VideoCardID;

typedef std::vector< VideoInputChannel * >                  VideoInputChannelsVec;

typedef std::hash_map< VideoCardID, VideoInputChannel * >   VideoInputChannelsMap;
typedef std::hash_map< VideoCardID, AVFramePtr >            AVFramePtrMap;

typedef std::vector< OutputStaticData >                     OutputStaticDataVec;
typedef std::hash_map< unsigned int, unsigned int >         UintUintMapping;

} // nrl
} // bv
