#include "stdafx.h"

#include "PluginOperations.h"

namespace bv {

// ***********************
//
AddPluginOperation::AddPluginOperation  ( model::IModelNodePtr parent, model::IPluginPtr plugin, int idx )
    : m_parent( parent ), m_plugin( plugin ), m_idx( idx )
{
}

// ***********************
//
bool            AddPluginOperation::Undo                ( BVProjectEditor * editor )
{
    editor; assert( false );
    return false;
}

// ***********************
//
bool            AddPluginOperation::Redo                ( BVProjectEditor * editor )
{
    editor; assert( false );
    return false;
}


// ***********************
//
DeletePluginOperation::DeletePluginOperation ( model::IModelNodePtr parent, model::IPluginPtr plugin, int idx )
    : m_parent( parent ), m_plugin( plugin ), m_idx( idx )
{
}

// ***********************
//
bool            DeletePluginOperation::Undo                ( BVProjectEditor * editor )
{
    editor; assert( false );
    return false;
}
    
// ***********************
//
bool            DeletePluginOperation::Redo                ( BVProjectEditor * editor )
{
    editor; assert( false );
    return false;
}

}
