#include "Framework/FrameworkTest.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Framework/BVTestAppLogic.h"
#include "Engine/Models/NodeLogics/NodeLogicFactory.h"

#include "Utils/Nodes/TestNodesCreator.h"
#include "Utils/Scenes/TestScenesCreator.h"

#include "Engine/Models/NodeLogics/Follow/Follow.h"


using namespace bv;


// ***********************
// One node follows another. Disable y-axis - logic shouldn't change y component of translation.
class DisableAxis : public bv::FrameworkTest
{
    DECALRE_GTEST_INFO_WITH_CONSTRUCTOR( DisableAxis )
public:

    virtual void        PreEvents           () override;
    virtual void        PreRender           () override;

    void                Initialize          ();

private:

    nodelogic::FollowPtr        m_follow;
    model::BasicNodePtr         m_following;
    model::BasicNodePtr         m_followed;
};
REGISTER_FRAMEWORK_GTEST_INFO( DisableAxis, Logics_Follow, DisableAxis )



// ***********************
//
void        DisableAxis::PreEvents           ()
{
    if( GetFrameNumber() == 0 )
        Initialize();
}

// ***********************
//
void        DisableAxis::PreRender          ()
{
    if( GetFrameNumber() == 0 )
    {
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).x, 2.5f, 0.00001 );
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).y, 3.0f, 0.00001 );
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).z, 1.0f, 0.00001 );
    }
    else if( GetFrameNumber() == 1 )
    {
        // Check what happens in second frame with the same parameters
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).x, 2.5f, 0.00001 );
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).y, 3.0f, 0.00001 );
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).z, 1.0f, 0.00001 );

        // Set parameters for next frame.
        m_followed->GetFinalizePlugin()->GetParamTransform()->SetTranslation( glm::vec3( 1.5, 0.0, 1.0 ), 0.0f );
    }
    else if( GetFrameNumber() == 2 )
    {
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).x, 3.5f, 0.00001 );
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).y, 3.0f, 0.00001 );
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).z, 2.0f, 0.00001 );

        // Set parameters for next frame.
        model::SetParameter( m_follow->GetParameter( nodelogic::Follow::PARAMETERS::FOLLOW_Y ), 0.0f, true );
    }
    else if( GetFrameNumber() == 3 )
    {
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).x, 3.5f, 0.00001 );
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).y, 1.0f, 0.00001 );
        EXPECT_NEAR( m_following->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0 ).z, 2.0f, 0.00001 );
    }
}

// ***********************
//
void        DisableAxis::Initialize         ()
{
    m_follow = TestScenesCreator::RectFollowsRect( GetProjectEditor(), "RectFollowsRect", "followed", "following" );

    model::SetParameter( m_follow->GetParameter( nodelogic::Follow::PARAMETERS::FOLLOW_Y ), 0.0f, false );
    model::SetParameter( m_follow->GetParameter( nodelogic::Follow::PARAMETERS::OFFSET_X ), 0.0f, 2.0f );

    m_following = std::static_pointer_cast< model::BasicNode >( GetProjectEditor()->GetNode( "RectFollowsRect", "root/following" ) );
    m_followed = std::static_pointer_cast< model::BasicNode >( GetProjectEditor()->GetNode( "RectFollowsRect", "root/followed" ) );

    m_followed->GetFinalizePlugin()->GetParamTransform()->SetTranslation( glm::vec3( 0.5, 0.0, 0.0 ), 0.0f );
    m_following->GetFinalizePlugin()->GetParamTransform()->SetTranslation( glm::vec3( 0.0, 3.0, 0.0 ), 0.0f );
}


