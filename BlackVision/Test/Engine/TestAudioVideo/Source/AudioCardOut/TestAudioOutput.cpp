#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"

#include "Engine/Editors/BVProjectEditor.h"

#include "Utils/Mocks/FakeVideoCard/FakeVideoCard.h"
#include "Utils/Nodes/TestNodesCreator.h"



using namespace bv;


// ***********************
//
class VideoCardsAudioOutputTest : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO( VideoCardsAudioOutputTest )

public:

    explicit VideoCardsAudioOutputTest()
    {}

public:

    virtual void        PreEvents           () override;
    virtual void        PostRender          () override;


};
REGISTER_FRAMEWORK_GTEST_INFO( VideoCardsAudioOutputTest, Engine_Audio, VideoCardsAudioOutput )




// ***********************
//
void            VideoCardsAudioOutputTest::PreEvents   ()
{
    if( GetFrameNumber() == 0 )
    {
        GetProjectEditor()->AddScene( "FirstScene" );
        
        auto scene = GetProjectEditor()->GetModelScene( "FirstScene" );
        auto audioNode = TestNodesCreator::FakeAudioPlugin( GetProjectEditor()->GetSceneDefaultTimeline( scene ), "AudioNode" );

        ASSERT_TRUE( GetProjectEditor()->AddChildNode( scene, scene->GetRootNode(), audioNode ) );
    }
}


// ***********************
//
void            VideoCardsAudioOutputTest::PostRender   ()
{}
