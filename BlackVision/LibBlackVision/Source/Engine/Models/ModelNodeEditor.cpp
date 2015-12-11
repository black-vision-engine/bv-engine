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
BasicNodePtr			ModelNodeEditor::CopyNode				()
{
    auto node = m_node.lock();
	return std::static_pointer_cast< BasicNode >( node->Clone() );
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
	ReplaceTimeline( node, oldTimeline, newTimeline );
}

// *********************************************************************************************


// *******************************
//
void				ModelNodeEditor::ReplaceTimeline	( model::BasicNodePtr node, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline )
{
	if( node == nullptr ) return;

	auto plugins = node->GetPluginList();
	for( UInt32 i = 0; i < plugins->NumPlugins(); ++i )
	{
		ReplaceTimeline( plugins->GetPlugin( i ), oldTimeline, newTimeline );
	}

	//FIXME: update model in node effect
	//auto effect = node->GetNodeEffect();

	for( UInt32 i = 0; i < node->GetNumChildren(); ++i )
	{
		ReplaceTimeline( node->GetChild( i ), oldTimeline, newTimeline );
	}
}

// *******************************
//
void				ModelNodeEditor::ReplaceTimeline	( model::IPluginPtr plugin, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline )
{
	if( plugin == nullptr ) return;

	auto pluginModel = plugin->GetPluginParamValModel();
	if( pluginModel )
	{
		if( pluginModel->GetTimeEvaluator() == oldTimeline )
		{
			//FIXME: cast
			std::static_pointer_cast< model::DefaultPluginParamValModel >( pluginModel )->SetTimeEvaluator( newTimeline );
		}

		//update all models
		ReplaceTimeline( pluginModel->GetPluginModel(), oldTimeline, newTimeline );
		ReplaceTimeline( pluginModel->GetTransformChannelModel(), oldTimeline, newTimeline );
		ReplaceTimeline( pluginModel->GetVertexAttributesChannelModel(), oldTimeline, newTimeline );
		ReplaceTimeline( pluginModel->GetPixelShaderChannelModel(), oldTimeline, newTimeline );
		ReplaceTimeline( pluginModel->GetVertexShaderChannelModel(), oldTimeline, newTimeline );
		ReplaceTimeline( pluginModel->GetGeometryShaderChannelModel(), oldTimeline, newTimeline );
	}

	//update resource models
	//FIXME: cast const
	if( plugin->GetVertexShaderChannel() )
		ReplaceTimeline( std::const_pointer_cast< ITexturesData >( plugin->GetVertexShaderChannel()->GetTexturesData() ), oldTimeline, newTimeline );
	if( plugin->GetPixelShaderChannel() )
		ReplaceTimeline( std::const_pointer_cast< ITexturesData >( plugin->GetPixelShaderChannel()->GetTexturesData() ), oldTimeline, newTimeline );
	if( plugin->GetGeometryShaderChannel() )
		ReplaceTimeline( std::const_pointer_cast< ITexturesData >( plugin->GetGeometryShaderChannel()->GetTexturesData() ), oldTimeline, newTimeline );
	
}

// *******************************
//
void				ModelNodeEditor::ReplaceTimeline	( ITexturesDataPtr txData, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline )
{
	if( txData == nullptr ) return;

	for( auto & tx : txData->GetTextures() )
		ReplaceTimeline( tx->GetSamplerState(), oldTimeline, newTimeline );
	for( auto & anim : txData->GetAnimations() )
		ReplaceTimeline( anim->GetSamplerState(), oldTimeline, newTimeline );
	for( auto & font : txData->GetFonts() )
		ReplaceTimeline( font->GetStateModel(), oldTimeline, newTimeline );
}

// *******************************
//
void				ModelNodeEditor::ReplaceTimeline	( model::IParamValModelPtr paramValModel, const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline )
{
	if( paramValModel == nullptr ) return;

	for( auto & eval : paramValModel->GetEvaluators() )
	{
		for( auto & param : eval->GetParameters() )
		{
			if( param->GetTimeEvaluator() == oldTimeline )
			{
				param->SetTimeEvaluator( newTimeline );
			}
		}
	}
	
	for( auto & param : paramValModel->GetParameters() )
	{
		if( param->GetTimeEvaluator() == oldTimeline )
		{
			param->SetTimeEvaluator( newTimeline );
		}
	}
}

} //model
} //bv