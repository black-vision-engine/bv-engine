#include "gtest/gtest.h"
#include "Framework/FrameworkTest.h"
#include "Framework/BVTestAppLogic.h"

#include "Engine/Editors/BVProjectEditor.h"

#include "Utils/Mocks/FakeVideoCard/FakeVideoCard.h"
#include "Utils/Mocks/Plugins/Audio/FakeAudioPlayerPlugin.h"
#include "Utils/Nodes/TestNodesCreator.h"



using namespace bv;


// ***********************
//
class VideoCardsAudioOutputTest : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO( VideoCardsAudioOutputTest )

public:

    explicit VideoCardsAudioOutputTest()
        : m_fakeAudio( nullptr )
        , m_fakeVideoCard( nullptr )
    {}

public:

    virtual void        PreEvents           () override;
    virtual void        PostRender          () override;

private:

    model::FakeAudioPlayerPluginPtr     m_fakeAudio;
    videocards::FakeVideoCardPtr        m_fakeVideoCard;

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

        m_fakeAudio = std::static_pointer_cast< model::FakeAudioPlayerPlugin >( audioNode->GetPlugin( "FakeAudioPlugin" ) );
        m_fakeVideoCard = std::static_pointer_cast< videocards::FakeVideoCard >( GetAppLogic()->GetVideoCardManager()->GetVideoCard( 0 ) );

        ASSERT_NE( m_fakeAudio, nullptr );
        ASSERT_NE( m_fakeVideoCard, nullptr );
    }
}


// ***********************
//
void            VideoCardsAudioOutputTest::PostRender   ()
{}
