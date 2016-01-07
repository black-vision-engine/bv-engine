#include "ModelNodeEditor.h"

#include "Engine/Models/BVProjectTools.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"


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
BasicNodePtr			ModelNodeEditor::CopyNode       ()
{
    auto node = m_node.lock();
    return std::static_pointer_cast< BasicNode >( IModelNodePtr( node->Clone() ) );
}

// ********************************
//
bool			ModelNodeEditor::AddPlugin				( IPluginPtr plugin, unsigned int idx )
{
	auto node = m_node.lock();
    if( plugin )
    {
	    return node->GetPlugins()->AttachPlugin( plugin, idx );
    }
    return false;
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
		return plugin->Clone();
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
void				ModelNodeEditor::RefreshNode		( SceneNode * sceneNode, Renderer * renderer )
{
	auto node = m_node.lock();
	BVProjectTools::ClearSingleNode( sceneNode, renderer );
	BVProjectTools::SyncSingleNode( node, sceneNode );
}

// ********************************
//
void				ModelNodeEditor::ReplaceTimeline	( const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline )
{
	auto node = m_node.lock();
	
    //replace timeevaluators in parameters
    for( auto & param : node->GetParameters() )
    {
        if( param->GetTimeEvaluator() == oldTimeline )
		{
			param->SetTimeEvaluator( newTimeline );
		}
    }

    //replace timeevaluator in plugin model
    auto plugins = node->GetPluginList();
    for( UInt32 i = 0; i < plugins->NumPlugins(); ++i )
    {
        auto pluginModel = plugins->GetPlugin( i )->GetPluginParamValModel();
	    if( pluginModel )
	    {
		    if( pluginModel->GetTimeEvaluator() == oldTimeline )
		    {
			    //FIXME: cast
			    std::static_pointer_cast< model::DefaultPluginParamValModel >( pluginModel )->SetTimeEvaluator( newTimeline );
		    }
        }
    }

	for( UInt32 i = 0; i < node->GetNumChildren(); ++i )
	{
        node->GetChild( i )->GetModelNodeEditor()->ReplaceTimeline( oldTimeline, newTimeline );
	}
}

} //model
} //bv