#include "stdafxBVApp.h"

#include "TestEditorsKeyboardHandler.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/Json/JsonSerializeObject.h"

#include "TestingScenes/MockScenes.h"
#include "BVAppLogic.h"
#include "BVGL.h"

namespace bv {

namespace {

// *********************************
//
model::IModelNodePtr CreateTestModelNodeInSomeSpecificScope( BVAppLogic * logic, const std::string & name )
{
    auto pluginsManager = logic->GetPluginsManager();
	auto timelineManager = model::TimelineManager::GetInstance();
	auto globalTimeline = timelineManager->GetRootTimeline();

    model::BasicNodePtr node = TestScenesFactory::CreateTestRandomNode( name, pluginsManager, globalTimeline );

    return node;
}

} // anonymous

// *********************************
//
void    TestEditorsKeyboardHandler::HandleKey   ( unsigned char c, BVAppLogic * logic )
{
	if( c == 's' )
    {
        BVXMLSerializer ser;
        //auto sob = new JsonSerializeObject();
        logic->GetBVProject()->GetModelScenes()[ 0 ]->Serialize( ser );
        ser.Save( "test.xml" );
    }
}

} //bv
