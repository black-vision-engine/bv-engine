#include "stdafx.h"
#include "DeleteTimelineOperation.h"

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
DeleteTimelineOperation::DeleteTimelineOperation( const std::string & sceneName, model::ITimeEvaluatorPtr parent, model::ITimeEvaluatorPtr deleted  )
    :   m_sceneName( sceneName )
    ,   m_parentTimeline( parent )
    ,   m_deletedTimeline( deleted )
    ,   m_forceDelete( false )
{}

// ***********************
//
DeleteTimelineOperation::DeleteTimelineOperation( const std::string & sceneName, model::ITimeEvaluatorPtr parent, model::ITimeEvaluatorPtr deleted, bool force, const std::string & replaceTimeline  )
    :   m_sceneName( sceneName )
    ,   m_parentTimeline( parent )
    ,   m_deletedTimeline( deleted )
    ,   m_forceDelete( force )
    ,   m_replaceTimeline( replaceTimeline )
{}

// ***********************
//
bool        DeleteTimelineOperation::Undo    ( BVProjectEditor * editor )
{
    return editor->AddTimeline( m_parentTimeline, m_deletedTimeline );
}

// ***********************
//
bool        DeleteTimelineOperation::Redo    ( BVProjectEditor * editor )
{    
    auto timelinePath = model::TimelineHelper::GetTimelinePath( m_deletedTimeline, m_parentTimeline );

    if( m_forceDelete )
        return editor->ForceDeleteTimeline( timelinePath, m_replaceTimeline );
    else
        return editor->DeleteTimeline( timelinePath );
}


}	// bv
