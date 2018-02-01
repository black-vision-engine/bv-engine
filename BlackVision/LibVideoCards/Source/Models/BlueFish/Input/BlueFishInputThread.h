#pragma once


#include <atomic>
#include <mutex>
#include <condition_variable>

#include "Threading/StoppableThread.h"
#include "DataTypes/QueueConcurrentLimited.h"

#include "Memory/AVFrame.h"
#include "DataTypes/Reusable.h"





namespace bv {
namespace videocards {
namespace bluefish
{


class InputChannel;
class VideoCard;
class CFrame;
DEFINE_PTR_TYPE( CFrame )


// ***********************
//
class BlueFishInputThread : public StoppableThread
{
    typedef QueueConcurrentLimited< AVFramePtr >        FrameQueue;
    typedef boost::circular_buffer< MemoryChunkPtr >    CircularBuffer;

private:

    FrameQueue					m_processedFrameQueue;
    InputChannel *              m_inputChannel;

    Reusable< MemoryChunkPtr >  m_reusableChunks;

public:

    explicit					BlueFishInputThread		( InputChannel * vc );

    virtual void				Process					() override;

    void						EnqueueEndMessage		();
    AVFramePtr                  PopNextFrame            ();

protected:

    MemoryChunkPtr              Deinterlace             ( const CFramePtr & videoChunk );

    void                        DeinterlaceLinear       ( const CFramePtr & inputChunk, MemoryChunkPtr outputChunk );
};

DEFINE_UPTR_TYPE( BlueFishInputThread )


}
}   // videocards
}	// bv




