#include "stdafx.h"
#include "AddTimelineOperation.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/Timeline/TimelineHelper.h"
#include "Engine/Models/Timeline/TimelineManager.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{


// ***********************
//
AddTimelineOperation::AddTimelineOperation( const std::string & sceneName, model::ITimeEvaluatorPtr parent, model::ITimeEvaluatorPtr added )
    :   m_sceneName( sceneName )
    ,   m_parentTimeline( parent )
    ,   m_addedTimeline( added )
{}

// ***********************
//
bool        AddTimelineOperation::Undo    ( BVProjectEditor * editor )
{
    auto timelinePath = model::TimelineHelper::GetTimelinePath( m_addedTimeline, m_parentTimeline );
    return editor->DeleteTimeline( timelinePath );
}

// ***********************
//
bool        AddTimelineOperation::Redo    ( BVProjectEditor * editor )
{
    return editor->AddTimeline( m_parentTimeline, m_addedTimeline );
}


}	// bv
