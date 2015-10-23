#include "TestEditorsKeyboardHandler.h"

#include "Engine/Models/BVSceneEditor.h"
#include "Serialization/XML/XMLSerializer.h"

#include "MockScenes.h"
#include "BVAppLogic.h"
#include "BVGL.h"

namespace bv {

namespace {

// *********************************
//
model::IModelNodePtr CreateTestModelNodeInSomeSpecificScope( BVAppLogic * logic, const std::string & name )
{
    auto pluginsManager = logic->GetPluginsManager();
    auto timelineManager = logic->GetTimelineManager();
    auto globalTimeline = logic->GetGlobalTimeline();

    model::BasicNodePtr node = TestScenesFactory::CreateTestRandomNode( name, pluginsManager, timelineManager, globalTimeline );

    return node;
}

} // anonymous

// *********************************
//
void    TestEditorsKeyboardHandler::HandleKey   ( unsigned char c, BVAppLogic * logic )
{
    auto m_bvScene = logic->GetBVScene();

    if( c == '-' )
    {
        BVGL::PrintCompleteSummary( "BEFORE REMOVING ROOT NODE" );
        m_bvScene->GetSceneEditor()->DeleteRootNode();
    }
    else if( c == 8 )
    {
        BVGL::PrintCompleteSummary( "BEFORE REMOVING ROOT NODE" );

        auto root = m_bvScene->GetModelSceneRoot();
        m_bvScene->GetSceneEditor()->DeleteChildNode( root, "child0" );
        //root->DeleteNode( "child0", m_renderer );
        
        //auto child = root->GetChild( "child0" );
        //child->DeleteNode( "child01", m_renderer );
        BVGL::PrintCompleteSummary( "AFTER REMOVING ROOT NODE" );
    }
    else if( c == '+' )
    {
        auto root = m_bvScene->GetModelSceneRoot();

        if( root )
        {
            auto child = root->GetChild( "child0" );

            if( child )
            {
                auto n = child->GetNumChildren();
                auto nodeName = "child0" + std::to_string(n);

                auto newNode = CreateTestModelNodeInSomeSpecificScope( logic, nodeName );

                m_bvScene->GetSceneEditor()->AddChildNode( child, newNode );            
            }
            else
            {
                auto newNode = CreateTestModelNodeInSomeSpecificScope( logic, "child0" );
                
                m_bvScene->GetSceneEditor()->AddChildNode( root, newNode );            
            }
        }
        else
        {
            auto newNode = CreateTestModelNodeInSomeSpecificScope( logic, "root node" );
        
            m_bvScene->GetSceneEditor()->SetRootNode( newNode );
        }

        BVGL::PrintCompleteSummary( "AFTER ADD NODE" );
    }
    else if( c == '1' )
    {
        m_bvScene->GetSceneEditor()->DetachRootNode();
        BVGL::PrintCompleteSummary( "AFTER DETACH ROOT NODE" );
    }
    else if( c == '2' )
    {
        m_bvScene->GetSceneEditor()->AttachRootNode();
        BVGL::PrintCompleteSummary( "AFTER ATTACH ROOT NODE" );
    }

    else if( c == '3' )
    {
        auto root = m_bvScene->GetModelSceneRoot();
        
        if( root )
        {
            m_bvScene->GetSceneEditor()->AttachChildNode( root );

            BVGL::PrintCompleteSummary( "AFTER ATTACH NODE TO ROOT" );
        }
    }
    else if( c == 's' )
    {
        auto sob = new SerializeObject();
        m_bvScene->Serialize( *sob );
        sob->Save( "text.xml" );
        delete sob;
    }
}

} //bv
