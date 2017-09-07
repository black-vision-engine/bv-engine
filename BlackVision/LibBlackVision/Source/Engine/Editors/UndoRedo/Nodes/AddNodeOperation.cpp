#include "stdafx.h"
#include "AddNodeOperation.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Editors/BVProjectEditor.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{


// ***********************
//
AddNodeOperation::AddNodeOperation( model::SceneModelPtr scene, model::IModelNodePtr parent, model::IModelNodePtr addedNode )
    :   m_scene( scene )
    ,   m_parentNode( parent )
    ,   m_addedNode( addedNode )
{}

// ***********************
//
bool        AddNodeOperation::Undo    ( BVProjectEditor * editor )
{
    return editor->DeleteChildNode( m_scene, m_parentNode, m_addedNode );
}

// ***********************
//
bool        AddNodeOperation::Redo    ( BVProjectEditor * editor )
{
    return editor->AddChildNode( m_scene, m_parentNode, m_addedNode );
}


}	// bv
