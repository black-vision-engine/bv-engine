#include "SceneEventsHandlers.h"

#include "ProjectManager.h"

namespace bv
{

// ***********************
//
SceneEventsHandlers::SceneEventsHandlers( BVAppLogic* /*appLogic*/ )
{}

SceneEventsHandlers::~SceneEventsHandlers()
{}

// ***********************
//
void SceneEventsHandlers::SceneStructure      ( bv::IEventPtr /*evt*/ )
{
    //if( evt->GetEventType() != bv::SceneStructureEvent::Type() )
    //    return;
    //bv::SceneStructureEventPtr evtStructure = std::static_pointer_cast<bv::SceneStructureEvent>( evt );

    //auto pm = ProjectManager::GetInstance();



}



} //bv
