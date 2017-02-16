#pragma once

#include <hash_map>
#include <vector>

#include "AVFrame.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/Components/VideoInputChannel.h"


namespace bv { 
    
using videocards::AVFramePtr;

namespace nrl {

typedef unsigned int VideoCardID;

typedef std::vector< VideoInputChannel * >                  VideoInputChannelsVec;

typedef std::hash_map< VideoCardID, VideoInputChannel * >   VideoInputChannelsMap;
typedef std::hash_map< VideoCardID, AVFramePtr >            AVFramePtrMap;

} // nrl
} // bv
