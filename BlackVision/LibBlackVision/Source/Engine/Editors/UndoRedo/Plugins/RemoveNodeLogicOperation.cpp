#include "stdafx.h"
#include "RemoveNodeLogicOperation.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/INodeLogic.h"

#include "Engine/Editors/BVProjectEditor.h"



#include "Memory/MemoryLeaks.h"



namespace bv
{


// ***********************
//
RemoveNodeLogicOperation::RemoveNodeLogicOperation( model::SceneModelPtr scene, model::IModelNodePtr parent, model::INodeLogicPtr logic )
    :   m_scene( scene )
    ,   m_parentNode( std::static_pointer_cast< model::BasicNode >( parent ) )
    ,   m_logic( logic )
{}

// ***********************
//
bool        RemoveNodeLogicOperation::Undo    ( BVProjectEditor * editor )
{
    return editor->SetLogic( m_parentNode, m_logic );
}

// ***********************
//
bool        RemoveNodeLogicOperation::Redo    ( BVProjectEditor * editor )
{
    return editor->RemoveLogic( m_parentNode );
}


}	// bv
