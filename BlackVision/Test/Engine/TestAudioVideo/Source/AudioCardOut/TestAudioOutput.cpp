#include "AudioSignalPath.h"

#include <algorithm>

#undef max

using namespace bv;


// ========================================================================= //
// Test for 50 FPS frame generated from FakeAudioPlugin
// ========================================================================= //


// ***********************
// This test sends generated sawtooth audio signal from fake plugin and reads results value from
// video card.
class VideoCardsAudioOutput50HzTest : public AudioSignalPathTest
{
    DECALRE_GTEST_INFO( VideoCardsAudioOutput50HzTest )

public:

    explicit VideoCardsAudioOutput50HzTest()
    {}

public:

    virtual void        PreEvents           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( VideoCardsAudioOutput50HzTest, Engine_Audio, VideoCardsAudioOutput_50Hz )


void        VideoCardsAudioOutput50HzTest::PreEvents    ()
{
    AudioSignalPathTest::PreEvents();

    if( GetFrameNumber() == 0 )
    {
        m_fakeAudio->SetFPS( 50 );
    }
}

// ========================================================================= //
// Test for 60 FPS frame generated from FakeAudioPlugin
// ========================================================================= //

// ***********************
// This test sends generated sawtooth audio signal from fake plugin and reads results value from
// video card.
class VideoCardsAudioOutput20HzTest : public AudioSignalPathTest
{
    DECALRE_GTEST_INFO( VideoCardsAudioOutput20HzTest )

public:

    explicit VideoCardsAudioOutput20HzTest()
    {}

public:

    virtual void        PreEvents           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( VideoCardsAudioOutput20HzTest, Engine_Audio, VideoCardsAudioOutput_20Hz )


void        VideoCardsAudioOutput20HzTest::PreEvents    ()
{
    AudioSignalPathTest::PreEvents();

    if( GetFrameNumber() == 0 )
    {
        m_fakeAudio->SetFPS( 20 );
    }
}



