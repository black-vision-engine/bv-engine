#pragma once 
#include "CoreDEF.h"
#include "AVEncoder.h"
#include "Threading/StoppableThread.h"
#include "DataTypes/QueueConcurrent.h"

#include <functional>

struct AVFormatContext;

namespace bv 
{

namespace avencoder
{
struct OutputStream;

class AVEncoderThread : public StoppableThread
{
    QueueConcurrent< AVFrameConstPtr >      m_framesQueue;
    AVFormatContext *                       m_oc;
    OutputStream *                          m_videoOS;
    OutputStream *                          m_audioOS;

    AVEncoderThread( const AVEncoderThread & copy );
    const AVEncoderThread & operator=( const AVEncoderThread & copy );

    virtual void				Process         () override;

    std::function< void ( AVFrameConstPtr ) > m_frameCompleteCallback;

public:
    bool                        IsEmpty         () const;

    void                        EnqueueFrame    ( const AVFrameConstPtr & frame );

    AVEncoderThread           ( AVFormatContext * oc, OutputStream * videoOS, OutputStream * audioOS, std::function< void ( const AVFrameConstPtr & ) > frameCompleteCallback );
    virtual ~AVEncoderThread  ();
};

} // videoencoder 

} // bv
