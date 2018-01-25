#pragma once


#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Threading/StoppableThread.h"
#include "DataTypes/QueueConcurrentLimited.h"

#include "Memory/AVFrame.h"

#include <boost/circular_buffer.hpp>




namespace bv {
namespace videocards {
namespace bluefish
{


class InputChannel;
class VideoCard;


// ***********************
//
class BlueFishInputThread : public StoppableThread
{
    typedef QueueConcurrentLimited< AVFramePtr >        FrameQueue;
    typedef boost::circular_buffer< MemoryChunkPtr >    CircularBuffer;

private:

    FrameQueue					m_processedFrameQueue;
    InputChannel *              m_inputChannel;


public:

    explicit					BlueFishInputThread		( InputChannel * vc );

    virtual void				Process					() override;

    void						EnqueueEndMessage		();
    AVFramePtr                  PopNextFrame            ();

protected:

    MemoryChunkPtr              Deinterlace             ( MemoryChunkPtr videoChunk );

    void                        DeinterlaceLinear       ( MemoryChunkPtr inputChunk, MemoryChunkPtr outputChunk );
};

DEFINE_UPTR_TYPE( BlueFishInputThread )


}
}   // videocards
}	// bv




