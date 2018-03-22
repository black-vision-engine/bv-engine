#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"





void        CopyNodeCompare                     ( bv::model::BasicNodePtr srcNode, bv::model::BasicNodePtr dstNode );
void        CopyPluginCompare                   ( bv::model::IPluginPtr srcPlugin, bv::model::IPluginPtr dstPlugin );
void        CopyLogicCompare                    ( bv::model::INodeLogicPtr srcLogic, bv::model::INodeLogicPtr dstLogic );
void        CopyEffectCompare                   ( bv::model::IModelNodeEffectPtr srcEffect, bv::model::IModelNodeEffectPtr dstEffect );



// ***********************
//
SIMPLE_FRAMEWORK_TEST_IN_SUITE( BVProjectEditor_Node_Copy, CopyNodeInScene )
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto scene1Name = editor->GetModelScene( "FirstScene" )->GetName();

    // Copy nodes hierarchy as child of root node.
    editor->AddNodeCopy( scene1Name, "root", scene1Name, "root/Group1", false );

    auto sourceNode = std::static_pointer_cast< bv::model::BasicNode >( editor->GetNode( scene1Name, "root/#0" ) );
    auto destNode   = std::static_pointer_cast< bv::model::BasicNode >( editor->GetNode( scene1Name, "root/#2" ) );

    ASSERT_TRUE( sourceNode != nullptr );
    ASSERT_TRUE( destNode != nullptr );

    EXPECT_TRUE( destNode->GetNumChildren() == 4 );

    CopyNodeCompare( sourceNode, destNode );
}

// ***********************
//
void        CopyNodeCompare                     ( bv::model::BasicNodePtr srcNode, bv::model::BasicNodePtr dstNode )
{
    EXPECT_TRUE( srcNode->GetName() == dstNode->GetName() );
    EXPECT_TRUE( srcNode != dstNode );

    auto srcLogic = srcNode->GetLogic();
    auto dstLogic = dstNode->GetLogic();

    CopyLogicCompare( srcLogic, dstLogic );


    auto srcEffect = srcNode->GetNodeEffect();
    auto dstEffect = dstNode->GetNodeEffect();

    CopyEffectCompare( srcEffect, dstEffect );


    ASSERT_TRUE( srcNode->GetNumPlugins() == dstNode->GetNumPlugins() );

    for( unsigned int i = 0; i < srcNode->GetNumPlugins(); ++i )
    {
        auto srcPlugin = srcNode->GetPlugins()->GetPlugin( i );
        auto dstPlugin = dstNode->GetPlugins()->GetPlugin( i );

        CopyPluginCompare( srcPlugin, dstPlugin );
    }


    ASSERT_TRUE( srcNode->GetNumChildren() == dstNode->GetNumChildren() );

    for( unsigned int i = 0; i < srcNode->GetNumChildren(); ++i )
    {
        CopyNodeCompare( srcNode->GetChild( i ), dstNode->GetChild( i ) );
    }
}

// ***********************
//
void        CopyPluginCompare                   ( bv::model::IPluginPtr srcPlugin, bv::model::IPluginPtr dstPlugin )
{
    ASSERT_TRUE( srcPlugin->GetTypeUid() == dstPlugin->GetTypeUid() );
    EXPECT_TRUE( srcPlugin->GetName() == dstPlugin->GetName() );

    // Compare assets
    ASSERT_TRUE( srcPlugin->GetLAssets().size() == dstPlugin->GetLAssets().size() );

    // FIXME: Add comparision code here
}

// ***********************
//
void        CopyLogicCompare                    ( bv::model::INodeLogicPtr srcLogic, bv::model::INodeLogicPtr dstLogic )
{
    ASSERT_TRUE( srcLogic == dstLogic );

    if( srcLogic != nullptr && dstLogic != nullptr )
    {
        EXPECT_TRUE( srcLogic->GetType() == dstLogic->GetType() );
        // FIXME: Add comparision code here
    }
}

// ***********************
//
void        CopyEffectCompare                   ( bv::model::IModelNodeEffectPtr srcEffect, bv::model::IModelNodeEffectPtr dstEffect )
{
    ASSERT_TRUE( srcEffect == dstEffect );

    if( srcEffect != nullptr && dstEffect != nullptr )
    {
        EXPECT_TRUE( srcEffect->GetType() == dstEffect->GetType() );
        EXPECT_TRUE( srcEffect->IsEnabled() == dstEffect->IsEnabled() );

        // FIXME: Add comparision code here
    }
}
