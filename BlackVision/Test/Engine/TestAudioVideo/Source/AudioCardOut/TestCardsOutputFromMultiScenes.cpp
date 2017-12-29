#include "AudioSignalPath.h"

#include <algorithm>

#undef max

using namespace bv;



// ***********************
/// Bug https://www.pivotaltracker.com/story/show/153849263 replication.
/// Audio from only one scene is rendered to output at the same time.
/// The problem is RenderLogic which clears AudioRendererChannel after each scene rendering phase.
/// If many scenes are rendered to the same render channel, it should be cleared only once.
class VideoCardsAudioOutput_MultiScenesTest : public AudioSignalPathTest
{
    DECALRE_GTEST_INFO( VideoCardsAudioOutput_MultiScenesTest )

public:

    explicit VideoCardsAudioOutput_MultiScenesTest()
        : AudioSignalPathTest( 10 )
    {}

protected:
public:

    virtual void        PreEvents           () override;

};
REGISTER_FRAMEWORK_GTEST_INFO( VideoCardsAudioOutput_MultiScenesTest, Engine_Audio, VideoCardsAudioOutput_MultiScenes )


// ***********************
//
void            VideoCardsAudioOutput_MultiScenesTest::PreEvents    ()
{
    AudioSignalPathTest::PreEvents();

    if( GetFrameNumber() == 0 )
    {
        // We add second scene which will clear audio from first scene if error exists.
        GetProjectEditor()->AddScene( "SecondScene" );
    }
}

