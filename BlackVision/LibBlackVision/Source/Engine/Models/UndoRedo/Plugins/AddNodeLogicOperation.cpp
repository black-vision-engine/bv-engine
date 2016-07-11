#include "stdafx.h"
#include "AddNodeLogicOperation.h"

#include "Engine/Models/SceneModel.h"
#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Interfaces/INodeLogic.h"

#include "Engine/Models/BVProjectEditor.h"

namespace bv
{


// ***********************
//
AddNodeLogicOperation::AddNodeLogicOperation( model::SceneModelPtr scene, model::IModelNodePtr parent, model::INodeLogicPtr logic )
    :   m_scene( scene )
    ,   m_parentNode( std::static_pointer_cast< model::BasicNode >( parent ) )
    ,   m_logic( logic )
{}

// ***********************
//
bool        AddNodeLogicOperation::Undo    ( BVProjectEditor * editor )
{
    return editor->RemoveLogic( m_parentNode );
}

// ***********************
//
bool        AddNodeLogicOperation::Redo    ( BVProjectEditor * editor )
{
    return editor->SetLogic( m_parentNode, m_logic );
}


}	// bv
