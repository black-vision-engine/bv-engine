#include "TestEditorsKeyboardHandler.h"

#include "Engine/Models/BVSceneEditor.h"
#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/Json/JsonSerializeObject.h"

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
	auto timelineManager = TimelineManager::GetInstance();
	auto globalTimeline = timelineManager->GetRootTimeline();

    model::BasicNodePtr node = TestScenesFactory::CreateTestRandomNode( name, pluginsManager, timelineManager, globalTimeline );

    return node;
}

} // anonymous

// *********************************
//
void    TestEditorsKeyboardHandler::HandleKey   ( unsigned char c, BVAppLogic * logic )
{
	if( c == 's' )
    {
        auto sob = new XMLSerializer();
        //auto sob = new JsonSerializeObject();
        logic->GetBVScene()->GetScenes()[ 0 ]->Serialize( *sob );
        sob->Save( "test.xml" );
        delete sob;
    }
}

} //bv
