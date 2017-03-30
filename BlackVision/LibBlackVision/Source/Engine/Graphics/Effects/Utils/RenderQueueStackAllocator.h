#pragma once

#include "CoreDEF.h"
#include "RenderingQueue.h"


#include <vector>

namespace bv
{



class RenderQueueStackAllocator
{
private:

    std::vector< RenderingQueueUPtr >   m_renderQueues;
    Int32                               m_currentIdx;

public:

    RenderQueueStackAllocator   ();
    ~RenderQueueStackAllocator  ();

    RenderingQueue *    Allocate                    ();
    RenderingQueue *    Top                         ();

    bool                Free                        ();
    bool                Free                        ( unsigned int num );

    unsigned int        GetTopIndex                 () const;

    void                Clear                       ();
};


}	// bv
