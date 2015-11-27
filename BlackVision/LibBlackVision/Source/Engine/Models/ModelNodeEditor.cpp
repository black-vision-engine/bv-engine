#include "ModelNodeEditor.h"

#include "Engine/Models/BVProjectTools.h"

#include "Engine/Models/Plugins/Plugin.h"

#include "Serialization/CloneViaSerialization.h"
#include "Assets/AssetDescsWithUIDs.h"

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
BasicNodePtr			ModelNodeEditor::CopyNode				()
{
    auto node = m_node.lock();
	//FIXME
	auto oldAssets = AssetDescsWithUIDs::GetInstance();

	AssetDescsWithUIDs assets;
	GetAssetsWithUIDs( assets, node );
	AssetDescsWithUIDs::SetInstance( assets );

	auto clone = bv::CloneViaSerialization::Clone( node.get(), "node" );

	AssetDescsWithUIDs::SetInstance( oldAssets ); //necessary?

	return clone;
}

// ********************************
//
bool			ModelNodeEditor::AddPlugin				( IPluginPtr plugin, unsigned int idx )
{
	auto node = m_node.lock();
	return node->GetPlugins()->AttachPlugin( plugin, idx );
}

// ********************************
//
bool			ModelNodeEditor::DeletePlugin				( const std::string & name )
{
	auto node = m_node.lock();
	auto plugin = node->GetPlugins()->DetachPlugin( name );
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
	auto node = m_node.lock();
	auto plugin = node->GetPlugins()->DetachPlugin( idx );
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
	auto node = m_node.lock();
	if( node->GetPlugins()->AttachPlugin( m_detachedPlugin, idx ) )
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
	auto node = m_node.lock();
	auto plugin = node->GetPlugins()->DetachPlugin( name );

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
	auto node = m_node.lock();
	auto plugin = node->GetPlugins()->DetachPlugin( idx );

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

// ********************************
//
IPluginPtr			ModelNodeEditor::CopyPlugin				( const std::string & name )
{
    auto node = m_node.lock();
	auto plugin = node->GetPlugin( name );

	if( plugin )
	{
		//FIXME
		auto oldAssets = AssetDescsWithUIDs::GetInstance();

		AssetDescsWithUIDs assets;
		GetAssetsWithUIDs( assets, node );
		AssetDescsWithUIDs::SetInstance( assets );

		auto clone = bv::CloneViaSerialization::Clone( std::static_pointer_cast< BasePlugin< IPlugin > >( plugin ).get(), "plugin" );

		AssetDescsWithUIDs::SetInstance( oldAssets ); //necessary?

		return clone;
	}
    return nullptr;
}

// *******************************
//
IModelNodeEffectPtr	ModelNodeEditor::GetNodeEffect		()
{
	auto node = m_node.lock();
	return node->GetNodeEffect();
}

// *******************************
//
void				ModelNodeEditor::SetNodeEffect		( IModelNodeEffectPtr nodeEffect )
{
	auto node = m_node.lock();
	node->SetNodeEffect( nodeEffect );
}

// ********************************
//
void				ModelNodeEditor::RefreshNode ( SceneNode * sceneNode, Renderer * renderer )
{
	auto node = m_node.lock();
	BVProjectTools::ClearSingleNode( sceneNode, renderer );
	BVProjectTools::SyncSingleNode( node, sceneNode );
}


} //model
} //bv