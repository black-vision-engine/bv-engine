#include "Framework/FrameworkTest.h"

#include "Utils/TwoScenesFixture.h"
#include "Framework/BVTestAppLogic.h"




// ***********************
// Test BVProjectEditor API for copying nodes.
class CopyNodeInScene : public bv::FrameworkTest
{
private:
public:
    CopyNodeInScene() : bv::FrameworkTest( "CopyNodeInScene", "BVProjectEditor.Node.Copy", __FILE__, __LINE__ ) {}

    virtual void        PreEvents           () override;

} CopyNodeInSceneInstance;

UnitTest::ListAdder adderCopyNodeInScene ( UnitTest::Test::GetTestList(), &CopyNodeInSceneInstance );


void        CopyNodeCompare                     ( bv::model::BasicNodePtr srcNode, bv::model::BasicNodePtr dstNode );
void        CopyPluginCompare                   ( bv::model::IPluginPtr srcPlugin, bv::model::IPluginPtr dstPlugin );
void        CopyLogicCompare                    ( bv::model::INodeLogicPtr srcLogic, bv::model::INodeLogicPtr dstLogic );
void        CopyEffectCompare                   ( bv::model::IModelNodeEffectPtr srcEffect, bv::model::IModelNodeEffectPtr dstEffect );



// ***********************
//
void        CopyNodeInScene::PreEvents           ()
{
    auto editor = GetAppLogic()->GetBVProject()->GetProjectEditor();
    CreateOneScene( editor );

    auto scene1Name = editor->GetModelScene( "FirstScene" )->GetName();

    // Copy nodes hierarchy as child of root node.
    editor->AddNodeCopy( scene1Name, "root", scene1Name, "root/Group1", false );

    auto sourceNode = std::static_pointer_cast< bv::model::BasicNode >( editor->GetNode( scene1Name, "root/#0" ) );
    auto destNode   = std::static_pointer_cast< bv::model::BasicNode >( editor->GetNode( scene1Name, "root/#2" ) );

    REQUIRE( sourceNode != nullptr );
    REQUIRE( destNode != nullptr );

    CHECK( destNode->GetNumChildren() == 4 );

    CopyNodeCompare( sourceNode, destNode );
}

// ***********************
//
void        CopyNodeCompare                     ( bv::model::BasicNodePtr srcNode, bv::model::BasicNodePtr dstNode )
{
    CHECK( srcNode->GetName() == dstNode->GetName() );
    CHECK( srcNode != dstNode );

    auto srcLogic = srcNode->GetLogic();
    auto dstLogic = dstNode->GetLogic();

    CopyLogicCompare( srcLogic, dstLogic );


    auto srcEffect = srcNode->GetNodeEffect();
    auto dstEffect = dstNode->GetNodeEffect();

    CopyEffectCompare( srcEffect, dstEffect );


    REQUIRE( srcNode->GetNumPlugins() == dstNode->GetNumPlugins() );

    for( unsigned int i = 0; i < srcNode->GetNumPlugins(); ++i )
    {
        auto srcPlugin = srcNode->GetPlugins()->GetPlugin( i );
        auto dstPlugin = dstNode->GetPlugins()->GetPlugin( i );

        CopyPluginCompare( srcPlugin, dstPlugin );
    }


    REQUIRE( srcNode->GetNumChildren() == dstNode->GetNumChildren() );

    for( unsigned int i = 0; i < srcNode->GetNumChildren(); ++i )
    {
        CopyNodeCompare( srcNode->GetChild( i ), dstNode->GetChild( i ) );
    }
}

// ***********************
//
void        CopyPluginCompare                   ( bv::model::IPluginPtr srcPlugin, bv::model::IPluginPtr dstPlugin )
{
    REQUIRE( srcPlugin->GetTypeUid() == dstPlugin->GetTypeUid() );
    CHECK( srcPlugin->GetName() == dstPlugin->GetName() );

    // Compare assets
    REQUIRE( srcPlugin->GetLAssets().size() == dstPlugin->GetLAssets().size() );

    // FIXME: Add comparision code here
}

// ***********************
//
void        CopyLogicCompare                    ( bv::model::INodeLogicPtr srcLogic, bv::model::INodeLogicPtr dstLogic )
{
    REQUIRE( srcLogic == dstLogic );

    if( srcLogic != nullptr && dstLogic != nullptr )
    {
        CHECK( srcLogic->GetType() == dstLogic->GetType() );
        // FIXME: Add comparision code here
    }
}

// ***********************
//
void        CopyEffectCompare                   ( bv::model::IModelNodeEffectPtr srcEffect, bv::model::IModelNodeEffectPtr dstEffect )
{
    REQUIRE( srcEffect == dstEffect );

    if( srcEffect != nullptr && dstEffect != nullptr )
    {
        CHECK( srcEffect->GetType() == dstEffect->GetType() );
        CHECK( srcEffect->IsEnabled() == dstEffect->IsEnabled() );

        // FIXME: Add comparision code here
    }
}
