#include "stdafx.h"
#include "AssignTimelineOperation.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Models/Timeline/TimelineManager.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{


// ***********************
//
AssignTimelineOperation::AssignTimelineOperation( const std::string & sceneName, model::ITimeEvaluatorPtr timeline, model::ITimeEvaluatorPtr prevTimeline, model::IParameterPtr param )
    :   m_sceneName( sceneName )
    ,   m_timeline( timeline )
    ,   m_prevTimeline( prevTimeline )
    ,   m_param( param )
{}

// ***********************
//
bool        AssignTimelineOperation::Undo    ( BVProjectEditor * editor )
{
    return editor->AssignTimeline( m_sceneName, m_param, m_prevTimeline );
}

// ***********************
//
bool        AssignTimelineOperation::Redo    ( BVProjectEditor * editor )
{
    return editor->AssignTimeline( m_sceneName, m_param, m_timeline );
}


}	// bv
