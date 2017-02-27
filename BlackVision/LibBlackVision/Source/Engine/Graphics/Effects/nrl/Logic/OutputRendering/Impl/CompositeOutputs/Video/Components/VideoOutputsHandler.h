#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Impl/CompositeOutputs/Video/VideoTypes.h"


namespace bv { namespace nrl {
   
class AVOutputsData;

class VideoOutputsHandler
{
private:


public:

    void    HandleOutputsData   ( const AVOutputsData & outputs );

};

} //bv
} //nrl
