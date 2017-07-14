#include "stdafx.h"
#include "RemoveNodeOperation.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Editors/BVProjectEditor.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{


// ***********************
//
RemoveNodeOperation::RemoveNodeOperation( model::SceneModelPtr scene, model::IModelNodePtr parent, model::IModelNodePtr removedNode )
    :   m_scene( scene )
    ,   m_parentNode( parent )
    ,   m_removedNode( removedNode )
{}

// ***********************
//
bool        RemoveNodeOperation::Undo    ( BVProjectEditor * editor )
{
    return editor->AddChildNode( m_scene, m_parentNode, m_removedNode );
}

// ***********************
//
bool        RemoveNodeOperation::Redo    ( BVProjectEditor * editor )
{
    return editor->DeleteChildNode( m_scene, m_parentNode, m_removedNode );    
}


}	// bv
