#pragma once

#include "Threading/StoppableThread.h"

#include "AVFrame.h"

#include "Serialization/XML/XMLDeserializer.h"

#include <boost/circular_buffer.hpp>

namespace bv { namespace videocards {

class VideoCardManager;

class TestVideoCardsMainThread : public StoppableThread
{
private:
    XMLDeserializer         m_deserializer;

    VideoCardManager &      m_vcm;

    UInt64                  m_lastQueuedFrameTime;

    boost::circular_buffer< AVFramePtr > m_buffer;

public:

    void        Process                     () override;

    explicit    TestVideoCardsMainThread    ();
    virtual     ~TestVideoCardsMainThread   () {};


};

} // videocards 
} // bv