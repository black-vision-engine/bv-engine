#pragma once

#include "RenderingQueue.h"


#include <vector>

namespace bv
{



class RenderQueueStackAllocator
{
private:

    std::vector< RenderingQueue * >     m_renderQueues;

public:
    RenderQueueStackAllocator();
    ~RenderQueueStackAllocator();
};


}	// bv
