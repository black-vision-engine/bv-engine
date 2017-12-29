#include "AudioSignalPath.h"

#include <algorithm>

#undef max

using namespace bv;


// ========================================================================= //
// Test for 50 FPS frame generated from FakeAudioPlugin
// ========================================================================= //


// ***********************
/// Bug https://www.pivotaltracker.com/story/show/153848341 replication.
/// Audio output should be zero after unloading all scenes.
///
/// That was not the case becasue RenderLogic didn't cleaned AudioRenderChannel data after removing scene.
class VideoCardsAudioOutput_UnloadSceneTest : public AudioSignalPathTest
{
    DECALRE_GTEST_INFO( VideoCardsAudioOutput_UnloadSceneTest )

public:

    explicit VideoCardsAudioOutput_UnloadSceneTest()
        : AudioSignalPathTest( 1 )
        , m_numCleanFrames( 4 )
    {}

protected:

    UInt16          m_numCleanFrames;

public:

    virtual void        PreEvents           () override;
    virtual void        PostRender          () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( VideoCardsAudioOutput_UnloadSceneTest, Engine_Audio, VideoCardsAudioOutput_UnloadScene )



// ***********************
//
void        VideoCardsAudioOutput_UnloadSceneTest::PreEvents    ()
{
    if( GetFrameNumber() == m_numCleanBuffersFrames + m_numTestFramesDuration )
    {
        GetAppLogic()->UnloadScenes();
    }
    else
    {
        AudioSignalPathTest::PreEvents();

        if( GetFrameNumber() == 0 )
        {
            m_fakeAudio->SetFPS( 50 );
        }
    }
}

// ***********************
//
void        VideoCardsAudioOutput_UnloadSceneTest::PostRender   ()
{
    if( GetFrameNumber() > m_numCleanBuffersFrames + m_numTestFramesDuration &&
        GetFrameNumber() <= m_numCleanBuffersFrames + m_numTestFramesDuration + m_numCleanFrames )
    {
        auto & outputs = m_fakeVideoCard->AccessOutputs();
        auto & avFrame = outputs.Outputs.begin()->second;

        AggregateOutputSignal( avFrame );
    }

    if( GetFrameNumber() == m_numCleanBuffersFrames + m_numTestFramesDuration + m_numCleanFrames )
    {
        SizeType errors = 0;
        // Output signal should be zero in this frame.
        for( SizeType i = 0; i < m_outputSignal.size(); ++i )
        {
            if( m_outputSignal[ i ] != 0 )
                errors++;
        }

        EXPECT_EQ( errors, 0 );

        EndTestAfterThisFrame( true );
    }
}

