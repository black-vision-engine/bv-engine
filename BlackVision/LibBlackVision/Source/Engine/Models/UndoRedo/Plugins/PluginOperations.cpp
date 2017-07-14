#include "stdafx.h"

#include "PluginOperations.h"

#include "Engine/Editors/BVProjectEditor.h"



#include "Memory/MemoryLeaks.h"



namespace bv {

// ***********************
//
AddPluginOperation::AddPluginOperation  ( model::BasicNodePtr parent, model::IPluginPtr plugin, UInt32 idx )
    : m_parent( parent ), m_plugin( plugin ), m_idx( idx )
{
}

// ***********************
//
bool            AddPluginOperation::Undo                ( BVProjectEditor * editor )
{
    return editor->DetachPlugin( m_parent, m_idx );
}

// ***********************
//
bool            AddPluginOperation::Redo                ( BVProjectEditor * editor )
{
    return editor->AddPlugin( m_parent, m_plugin, m_idx );
}


// ***********************
//
DeletePluginOperation::DeletePluginOperation ( model::BasicNodePtr parent, model::IPluginPtr plugin, UInt32 idx )
    : m_parent( parent ), m_plugin( plugin ), m_idx( idx )
{
}

// ***********************
//
bool            DeletePluginOperation::Undo                ( BVProjectEditor * editor )
{
    return editor->AddPlugin( m_parent, m_plugin, m_idx );
}
    
// ***********************
//
bool            DeletePluginOperation::Redo                ( BVProjectEditor * editor )
{
    return editor->DetachPlugin( m_parent, m_idx );
}

}
