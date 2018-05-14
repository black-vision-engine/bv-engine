#include "Framework/FrameworkTest.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Framework/BVTestAppLogic.h"
#include "Engine/Models/NodeLogics/NodeLogicFactory.h"

#include "Utils/Nodes/TestNodesCreator.h"
#include "Utils/Scenes/TestScenesCreator.h"

#include "Engine/Models/NodeLogics/Follow/Follow.h"


using namespace bv;


// ***********************
// One node follows another. Check if following node alignment works properly.
class FollowerAlignment : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( FollowerAlignment )
public:

    virtual void        PreEvents           () override;
    virtual void        PreRender           () override;

    void                Initialize          ();

private:

    nodelogic::FollowPtr        m_follow;
    model::BasicNodePtr         m_following;
    model::BasicNodePtr         m_followed;
};
REGISTER_FRAMEWORK_GTEST_INFO( FollowerAlignment, Logics_Follow, FollowerAlignment )



// ***********************
//
void        FollowerAlignment::PreEvents        ()
{
    if( GetFrameNumber() == 0 )
        Initialize();
}

// ***********************
//
void        FollowerAlignment::PreRender        ()
{
    if( GetFrameNumber() == 0 )
    {
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).x, -0.5f, 0.00001 );
    }

}

// ***********************
//
void        FollowerAlignment::Initialize       ()
{
    m_follow = TestScenesCreator::RectFollowsRect( GetProjectEditor(), "RectFollowsRect", "followed", "following" );

    model::SetParameter( m_follow->GetParameter( nodelogic::Follow::PARAMETERS::OFFSET_X ), 0.0f, 0.0f );

    model::SetParameter( m_follow->GetParameter( nodelogic::Follow::PARAMETERS::FOLLOWER_ALIGN_X ), 0.0f, nodelogic::Follow::BBAlignementX::Right );
    model::SetParameter( m_follow->GetParameter( nodelogic::Follow::PARAMETERS::TARGET_ALIGN_X ), 0.0f, nodelogic::Follow::BBAlignementX::Left );

    m_following = std::static_pointer_cast< model::BasicNode >( GetProjectEditor()->GetNode( "RectFollowsRect", "root/following" ) );
    m_followed = std::static_pointer_cast< model::BasicNode >( GetProjectEditor()->GetNode( "RectFollowsRect", "root/followed" ) );

    m_followed->GetFinalizePlugin()->GetParamTransform()->SetTranslation( glm::vec3( 0.5, 0.0, 0.0 ), 0.0f );
    m_following->GetFinalizePlugin()->GetParamTransform()->SetTranslation( glm::vec3( 0.0, 3.0, 0.0 ), 0.0f );
}
