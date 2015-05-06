#include "ModelNodeEditor.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/BVSceneTools.h"

namespace bv { namespace model {

// *******************************
//
				ModelNodeEditor::ModelNodeEditor		( BasicNodePtr model )
    : m_model( model )
	, m_detachedPlugin( nullptr )
{
}

// ********************************
//
bool			ModelNodeEditor::AddPlugin				( IPluginPtr plugin )
{
	if( plugin && PluginCanBeAttached( plugin ) )
	{
		m_model->GetPlugins()->AttachPlugin( plugin );

		return true;
	}
	return false;
}

// ********************************
//
bool			ModelNodeEditor::DeletePlugin			( const std::string & name )
{
	return m_model->GetPlugins()->DetachPlugin( name );
}

// ********************************
//
bool				ModelNodeEditor::AttachPlugin			()
{
	if( m_detachedPlugin && PluginCanBeAttached( m_detachedPlugin ) )
    {
		m_model->GetPlugins()->AttachPlugin( m_detachedPlugin );
        m_detachedPlugin = nullptr;

		return true;
    }

	return false;
}

// ********************************
//
bool				ModelNodeEditor::DetachPlugin		( const std::string & name )
{
	m_detachedPlugin = m_model->GetPlugins()->DetachPlugin( name );

	if ( m_detachedPlugin )
	{
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
void				ModelNodeEditor::DeleteDetachedPlugin	()
{
    m_detachedPlugin = nullptr;
}


bool				ModelNodeEditor::PluginCanBeAttached ( IPluginPtr plugin )
{
	auto pluginsManager = &PluginsManager::DefaultInstance();
	auto plugins = m_model->GetPlugins();

	auto prev = plugins->NumPlugins() > 0 ? plugins->GetLastPlugin() : nullptr;

	assert( pluginsManager->CanBeAttachedTo( plugin->GetTypeUid(), prev ) );
		
	return pluginsManager->CanBeAttachedTo( plugin->GetTypeUid(), prev );
}

void				ModelNodeEditor::RefreshNode ( SceneNode * sceneNode, Renderer * renderer )
{
		BVSceneTools::ClearSingleNode( sceneNode, renderer );
		BVSceneTools::SyncSingleNode( m_model, sceneNode );
}


} //model
} //bv