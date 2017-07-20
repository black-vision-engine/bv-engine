#include "stdafx.h"

#include "LightOperations.h"

#include "Engine/Editors/BVProjectEditor.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// ***********************
//
AddLightOperation::AddLightOperation  ( model::SceneModelPtr parent, model::IModelLightPtr light )
    : m_parent( parent ), m_light( light )
{
}

// ***********************
//
bool            AddLightOperation::Undo                ( BVProjectEditor * /*editor*/ )
{
    return m_parent->RemoveLight( UInt32( m_parent->NumLights() ) - 1 );
}

// ***********************
//
bool            AddLightOperation::Redo                ( BVProjectEditor * /*editor*/ )
{
    m_parent->AddLight( m_light );
    return true;
}

// ***********************
//
DeleteLightOperation::DeleteLightOperation ( model::SceneModelPtr parent, model::IModelLightPtr light, int idx )
    : m_parent( parent ), m_light( light ), m_idx( idx )
{
}

// ***********************
//
bool            DeleteLightOperation::Undo                ( BVProjectEditor * /*editor*/ )
{
    m_idx = UInt32( m_parent->NumLights() ); // FIXME!!!!!!!!!!!!!!!!!!!!! we should put it in the right place instead of changing the index
    m_parent->AddLight( m_light ); 
    return false;
}

// ***********************
//
bool            DeleteLightOperation::Redo                ( BVProjectEditor * /*editor*/ )
{
    return m_parent->RemoveLight( m_idx );
}


}
