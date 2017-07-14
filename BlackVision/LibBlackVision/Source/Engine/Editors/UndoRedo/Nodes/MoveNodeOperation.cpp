#include "stdafx.h"
#include "MoveNodeOperation.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Editors/BVProjectEditor.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{


// ***********************
//
MoveNodeOperation::MoveNodeOperation(   model::SceneModelPtr scene,
                                        model::IModelNodePtr srcParent,
                                        model::IModelNodePtr destParent,
                                        model::IModelNodePtr movedNode,
                                        UInt32 srcIdx,
                                        UInt32 destIdx )
    :   m_scene( scene )
    ,   m_destParentNode( std::static_pointer_cast< model::BasicNode >( destParent ) )
    ,   m_srcParentNode( std::static_pointer_cast< model::BasicNode >( srcParent ) )
    ,   m_movedNode( std::static_pointer_cast< model::BasicNode >( movedNode ) )
    ,   m_srcIdx( srcIdx )
    ,   m_destIdx( destIdx )
{}

// ***********************
//
bool        MoveNodeOperation::Undo    ( BVProjectEditor * editor )
{
    return editor->MoveNode( m_scene, m_srcParentNode, m_srcIdx, m_scene, m_destParentNode, m_movedNode );
}

// ***********************
//
bool        MoveNodeOperation::Redo    ( BVProjectEditor * editor )
{
    return editor->MoveNode( m_scene, m_destParentNode, m_destIdx, m_scene, m_srcParentNode, m_movedNode );
}


}	// bv
