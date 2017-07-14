#include "stdafx.h"
#include "RenameTimelineOperation.h"

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
RenameTimelineOperation::RenameTimelineOperation( const std::string & timelinePath, const std::string & newName, const std::string & previousName  )
    :   m_timelinePath( timelinePath )
    ,   m_newName( newName )
    ,   m_previousName( previousName )
{}

// ***********************
//
bool        RenameTimelineOperation::Undo    ( BVProjectEditor * editor )
{
    return editor->RenameTimeline( m_timelinePath, m_previousName );
}

// ***********************
//
bool        RenameTimelineOperation::Redo    ( BVProjectEditor * editor )
{
    return editor->RenameTimeline( m_timelinePath, m_newName );
}


}	// bv
