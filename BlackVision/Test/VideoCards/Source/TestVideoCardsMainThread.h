#pragma once

#include "Threading/StoppableThread.h"

#include "AVFrame.h"

#include "Serialization/XML/XMLDeserializer.h"

namespace bv { namespace videocards {

class VideoCardManager;

class TestVideoCardsMainThread : public StoppableThread
{
private:
    XMLDeserializer         m_deserializer;

    VideoCardManager &      m_vcm;

    AVFramePtr              m_testFrame;

public:

    void        Process                     () override;

    explicit    TestVideoCardsMainThread    ( AVFramePtr testFrame = nullptr );
    virtual     ~TestVideoCardsMainThread   () {};


};

} // videocards 
} // bv