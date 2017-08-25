#include "stdafx.h"

#include "ModelNodeEditor.h"

#include "Engine/Editors/BVProjectTools.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"




#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *******************************
//
                ModelNodeEditor::ModelNodeEditor		( BasicNode * node )
    : m_node( node )
    , m_detachedPlugin( nullptr )
{
}

// ********************************
//
BasicNodePtr			ModelNodeEditor::CopyNode       ()
{
    return m_node->Clone();
}

// ********************************
//
bool			ModelNodeEditor::AddPlugin				( IPluginPtr plugin, UInt32& idx )
{
    if( plugin )
    {
        return m_node->GetPlugins()->AttachPlugin( plugin, idx );
    }
    return false;
}

// ********************************
//
model::PluginWithIdx			ModelNodeEditor::DeletePlugin				( const std::string & name )
{
    return m_node->GetPlugins()->DetachPlugin( name );
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
bool				ModelNodeEditor::AttachPlugin			( UInt32 & idx )
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
bool				ModelNodeEditor::AttachPlugin			( BasicNodePtr sourceNode, UInt32 & idx )
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

    if ( plugin.first )
    {
        m_detachedPlugin = plugin.first;
        m_detachedPluginIdx = plugin.second;
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


// ***********************
//
int                     ModelNodeEditor::GetDetachedPluginIdx ()
{
    return m_detachedPluginIdx;
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
    auto plugin = m_node->GetPlugin( name );

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
void				ModelNodeEditor::ReplaceTimeline	( const model::ITimeEvaluatorPtr & oldTimeline, model::ITimeEvaluatorPtr newTimeline )
{
    //replace timeevaluators in parameters
    for( auto & param : m_node->GetParameters() )
    {
        if( param->GetTimeEvaluator() == oldTimeline )
        {
            param->SetTimeEvaluator( newTimeline );
        }
    }

    // Replace default timeline in ParamVaModel
    auto pluginsList = m_node->GetPluginList();
    for( unsigned int i = 0; i < pluginsList->NumPlugins(); ++i )
    {
        auto plugin = pluginsList->GetPlugin( i );
        auto model = std::static_pointer_cast< DefaultPluginParamValModel >( plugin->GetPluginParamValModel() );

        if( model->GetTimeEvaluator() == oldTimeline )
            model->SetTimeEvaluator( newTimeline );
    }

    for( UInt32 i = 0; i < m_node->GetNumChildren(); ++i )
    {
        m_node->GetChild( i )->GetModelNodeEditor()->ReplaceTimeline( oldTimeline, newTimeline );
    }
}

// ***********************
//
bool                ModelNodeEditor::IsTimelineUsed   ( model::ITimeEvaluatorPtr timeEval )
{
    for( auto & param : m_node->GetParameters() )
    {
        if( param->GetTimeEvaluator() == timeEval )
        {
            return true;
        }
    }

    for( UInt32 i = 0; i < m_node->GetNumChildren(); ++i )
    {
        bool result = m_node->GetChild( i )->GetModelNodeEditor()->IsTimelineUsed( timeEval );
        if( result )
            return true;
    }

    return false;
}

} //model
} //bv