#include "ModelNodeEditor.h"

#include "Engine/Models/BVSceneTools.h"

namespace bv { namespace model {

// *******************************
//
				ModelNodeEditor::ModelNodeEditor		( BasicNodePtr node )
    : m_node( node )
	, m_detachedPlugin( nullptr )
{
}

// ********************************
//
bool			ModelNodeEditor::AddPlugin				( IPluginPtr plugin, unsigned int idx )
{
	return m_node->GetPlugins()->AttachPlugin( plugin, idx );
}

// ********************************
//
bool			ModelNodeEditor::DeletePlugin				( const std::string & name )
{
	auto plugin = m_node->GetPlugins()->DetachPlugin( name );
	if( plugin )
	{
		plugin = nullptr;
		return true;
	}
	return false;
}

// ********************************
//
bool			ModelNodeEditor::DeletePlugin				( unsigned int idx )
{
	auto plugin = m_node->GetPlugins()->DetachPlugin( idx );
	if( plugin )
	{
		plugin = nullptr;
		return true;
	}
	return false;
}

// ********************************
//
bool				ModelNodeEditor::AttachPlugin			( unsigned int idx )
{
	if( m_node->GetPlugins()->AttachPlugin( m_detachedPlugin, idx ) )
	{
		m_detachedPlugin = nullptr;
		return true;
	}
	return false;
}

// ********************************
//
bool				ModelNodeEditor::AttachPlugin			( BasicNodePtr sourceNode, unsigned int idx )
{
	auto plugin = sourceNode->GetModelNodeEditor()->GetDetachedPlugin();
	if( plugin )
	{
		if( AddPlugin( plugin, idx ) )
		{
			sourceNode->GetModelNodeEditor()->ResetDetachedPlugin();
			return true;
		}
	}
	return false;
}

// ********************************
//
bool				ModelNodeEditor::DetachPlugin			( const std::string & name )
{
	auto plugin = m_node->GetPlugins()->DetachPlugin( name );

	if ( plugin )
	{
		m_detachedPlugin = plugin;
		return true;
	}

    return false;
}

// ********************************
//
bool				ModelNodeEditor::DetachPlugin			( unsigned int idx )
{
	auto plugin = m_node->GetPlugins()->DetachPlugin( idx );

	if ( plugin )
	{
		m_detachedPlugin = plugin;
		return true;
	}

    return false;
}

// ********************************
//
IPluginPtr			ModelNodeEditor::GetDetachedPlugin	()
{
    return m_detachedPlugin;
}

// ********************************
//
void				ModelNodeEditor::ResetDetachedPlugin	()
{
    m_detachedPlugin = nullptr;
}

// *******************************
//
IModelNodeEffectPtr	ModelNodeEditor::GetNodeEffect		()
{
	return m_node->GetNodeEffect();
}

// *******************************
//
void				ModelNodeEditor::SetNodeEffect		( IModelNodeEffectPtr nodeEffect )
{
	m_node->SetNodeEffect( nodeEffect );
}

// ********************************
//
void				ModelNodeEditor::RefreshNode ( SceneNode * sceneNode, Renderer * renderer )
{
		BVSceneTools::ClearSingleNode( sceneNode, renderer );
		BVSceneTools::SyncSingleNode( m_node, sceneNode );
}


} //model
} //bv