#include "stdafx.h"
#include "NodeVisibilityAnimation.h"

#include "Serialization/SerializationHelper.h"
#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeMovedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeCopiedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Editors/BVProjectEditor.h"
#include "Engine/Editors/ModelSceneEditor.h"
#include "Engine/Models/SceneModel.h"

#include "Engine/Models/ModelState.h"

#include "Engine/Events/EventHandlerHelpers.h"

namespace bv
{
namespace nodelogic
{

const std::string       NodeVisibilityAnimation::m_type = "NodeVisibilityAnimation";

const std::string       NodeVisibilityAnimation::ACTION::REGISTER_NODE_VISIBILITY_PARAM = "RegisterNodeVisibilityParam";
const std::string       NodeVisibilityAnimation::ACTION::LIST_NODE_VISIBILITY_PARAMS = "ListNodeVisibilityParams";
const std::string       NodeVisibilityAnimation::ACTION::UNREGISTER_NODE_VISIBILITY_PARAM = "UnregisterNodeVisibilityParam";



namespace
{

const static std::string NODE_VISIBILITY_PARAM_NAME_PREFIX = "NodeVisibilityParam@";

}

// ***********************
//
const std::string &     NodeVisibilityAnimation::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     NodeVisibilityAnimation::GetType             () const
{
    return Type();
}

// ***********************
//
NodeVisibilityAnimation::NodeVisibilityAnimation             ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
    , m_defaultTimeEvaluator( timeEvaluator )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );
}

// ***********************
//
NodeVisibilityAnimation::~NodeVisibilityAnimation()
{}


// ***********************
//
void        NodeVisibilityAnimation::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &NodeVisibilityAnimation::NodeRemovedHandler ), NodeRemovedEvent::Type() );
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &NodeVisibilityAnimation::NodeMovedHandler ), NodeMovedEvent::Type() );

}

// ***********************
//
void        NodeVisibilityAnimation::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &NodeVisibilityAnimation::NodeRemovedHandler ), NodeRemovedEvent::Type() );
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &NodeVisibilityAnimation::NodeMovedHandler ), NodeMovedEvent::Type() );

}

// ***********************
//
void                        NodeVisibilityAnimation::Update			( TimeType t )
{
    NodeLogicBase::Update( t );

    for( auto & pn : m_paramNodes )
    {
        if( pn.first.Changed() )
        {
            if( auto node = pn.second.lock() )
                node->SetVisible( pn.first.GetValue() );
        }
    }
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        NodeVisibilityAnimation::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast< BVSerializeContext* >( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );
    ser.SetAttribute( "type", m_type );

    if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
    {
        NodeLogicBase::Serialize( ser );
    }

    ser.ExitChild();    // logic
}

// ***********************
//
void                        NodeVisibilityAnimation::Deserialize     ( const IDeserializer & deser )
{
    // params
    auto params = SerializationHelper::DeserializeArray< model::AbstractModelParameter >( deser, "params" );

    for( auto p : params )
    {
        auto nodePath = p->GetName().substr( p->GetName().find_first_of( '@' ) + 1 );
        RegisterNodeVisibilityParam( nodePath );

        m_paramValModel->SetParameter( p );
    }
}

// ***********************
//
NodeVisibilityAnimationPtr              NodeVisibilityAnimation::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< NodeVisibilityAnimation >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        NodeVisibilityAnimation::HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == NodeVisibilityAnimation::ACTION::REGISTER_NODE_VISIBILITY_PARAM )
    {
        auto context = static_cast< BVDeserializeContext * >( eventDeser.GetDeserializeContext() );
        auto sceneName = context->GetSceneName();
        auto rootPath = context->GetNodePath();
        auto nodePath = eventDeser.GetAttribute( "NodePath" );

        if( !rootPath.empty() && !nodePath.empty() && !sceneName.empty() )
        {
            if( RegisterNodeVisibilityParam( nodePath ) )
                return true;
            else
            {
                response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Replicate' command." );
                return false;
            }
        }
    }
    else if( action == NodeVisibilityAnimation::ACTION::LIST_NODE_VISIBILITY_PARAMS )
    {
        ListNodesVisibility( response );
        return true;
    }
    else if( action == NodeVisibilityAnimation::ACTION::UNREGISTER_NODE_VISIBILITY_PARAM )
    {
        auto context = static_cast< BVDeserializeContext * >( eventDeser.GetDeserializeContext() );
        auto sceneName = context->GetSceneName();
        auto rootPath = context->GetNodePath();
        auto nodePath = eventDeser.GetAttribute( "NodePath" );

        if( !rootPath.empty() && !nodePath.empty() && !sceneName.empty() )
        {
            if( auto parent = m_parentNode.lock() )
            {
                auto node = parent->GetNode( nodePath );
                RemoveNodeParam( node );
                
                return true;
            }
        }
    }
    else
        response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only ' "
            + NodeVisibilityAnimation::ACTION::REGISTER_NODE_VISIBILITY_PARAM + ", "
            + NodeVisibilityAnimation::ACTION::LIST_NODE_VISIBILITY_PARAMS + ", "
            + NodeVisibilityAnimation::ACTION::UNREGISTER_NODE_VISIBILITY_PARAM + " ' commands." );

    return false;
}

// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        NodeVisibilityAnimation::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;

    if( auto parentNode = m_parentNode.lock() )
    {
        auto typedEvent = QueryTypedEvent< NodeRemovedEvent >( evt );
        
        RemoveNodeParam( typedEvent->RemovedNode );
    }
}

// ========================================================================= //
// Handling moving of nodes
// ========================================================================= //

// ***********************
//
void                        NodeVisibilityAnimation::NodeMovedHandler   ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeMovedEvent::Type() )
        return;

	auto typedEvent = QueryTypedEvent< NodeMovedEvent >(evt);

    if( auto parentNode = m_parentNode.lock() )
		UpdateParamOnNodeMoving(typedEvent->Node);
}

// ========================================================================= //
// Handling copying of nodes
// ========================================================================= //

// ***********************
//
void                        NodeVisibilityAnimation::NodeCopieddHandler				( IEventPtr evt )
{
    if( evt->GetEventType() != NodeCopiedEvent::Type() )
        return;

	auto typedEvent = QueryTypedEvent< NodeCopiedEvent >(evt);

    if( auto parentNode = m_parentNode.lock() )
		UpdateParamOnNodeCopying( typedEvent->Node, typedEvent->SrcNode );
}

// ***********************
//
void                        NodeVisibilityAnimation::UpdateParamOnNodeMoving         ( const model::IModelNodePtr & movedNode )
{
    for( auto it = m_paramNodes.begin(); it != m_paramNodes.end(); ++it )
    {
        auto node = ( *it ).second.lock();
        if( node == movedNode )
        {
            auto newName = model::ModelState::GetInstance().QueryNodePath( movedNode.get() );
            const_cast< model::ParamBool * >( ( *it ).first.paramPtr )->SetName( NODE_VISIBILITY_PARAM_NAME_PREFIX + newName );
            const_cast< ValueBool * >( ( *it ).first.valuePtr )->SetName( NODE_VISIBILITY_PARAM_NAME_PREFIX + newName );
        }
    }

    for( UInt32 i = 0; i < movedNode->GetNumChildren(); ++i )
        UpdateParamOnNodeMoving( std::static_pointer_cast< model::BasicNode >( movedNode )->GetChild( i ) );
}

// ***********************
//
void                        NodeVisibilityAnimation::UpdateParamOnNodeCopying         ( const model::IModelNodePtr & copiedNode, const model::IModelNodePtr & srcNode )
{
    for( auto it = m_paramNodes.begin(); it != m_paramNodes.end(); ++it )
    {
        auto node = ( *it ).second.lock();
        if( node == srcNode )
        {
            auto newName = model::ModelState::GetInstance().QueryNodePath( copiedNode.get() );
			if( RegisterNodeVisibilityParam( newName ) )
				const_cast< model::ParamBool * >( m_paramNodes.back().first.paramPtr )->AccessInterpolator() = const_cast< model::ParamBool * >( ( *it ).first.paramPtr )->AccessInterpolator();
       }
    }

    for( UInt32 i = 0; i < copiedNode->GetNumChildren(); ++i )
		UpdateParamOnNodeCopying(	std::static_pointer_cast< model::BasicNode >( copiedNode )->GetChild( i ),
									std::static_pointer_cast< model::BasicNode >( srcNode )->GetChild( i ) );
}

// ***********************
//
void                        NodeVisibilityAnimation::RemoveNodeParam         ( const model::IModelNodePtr & removedNode )
{
    std::string paramName = "";

    for( auto it = m_paramNodes.begin(); it != m_paramNodes.end(); )
    {
        auto node = ( *it ).second.lock();
        if( node == removedNode )
        {
            paramName = ( *it ).first.GetParameter().GetName();
            it = m_paramNodes.erase( it );
        }
        else
        {
            it++;
        }
    }

    if( !paramName.empty() )
        m_paramValModel->RemoveParamVal( paramName );

    for( UInt32 i = 0; i < removedNode->GetNumChildren(); ++i )
        RemoveNodeParam( std::static_pointer_cast< model::BasicNode >( removedNode )->GetChild( i ) );
}

// ***********************
//
bool                        NodeVisibilityAnimation::RegisterNodeVisibilityParam( const std::string & nodePath )
{
    if( auto parent = m_parentNode.lock() )
    {
        auto node = parent->GetNode( nodePath );

        if( node )
        {
            // do not register scene root node
            if( node == parent )
                return false;

            // do not register if already registered.
            for( auto & pn : m_paramNodes )
            {
                if( pn.second.lock() == node )
                    return false;
            }

            auto plParamValModel = std::make_shared< model::DefaultPluginParamValModel >( m_defaultTimeEvaluator );
            plParamValModel->SetPluginModel( m_paramValModel );

            model::ModelHelper h( m_defaultTimeEvaluator, plParamValModel );
            h.SetOrCreatePluginModel();

            auto paramName = NODE_VISIBILITY_PARAM_NAME_PREFIX + nodePath;

            h.AddSimpleParam< bool >( paramName, node->IsVisible(), true, true );

            auto param = m_paramValModel->GetParameter( paramName );
            model::QueryTypedParam< model::ParamBoolPtr >( param )->SetGlobalCurveType( CurveType::CT_POINT );

            m_paramNodes.push_back( std::make_pair( model::GetValueParamState< bool >( m_paramValModel.get(), paramName ), node ) );

            return true;
        }
    }
    
    return false;
}

// ***********************
//
void                        NodeVisibilityAnimation::ListNodesVisibility( ISerializer & response )
{
    response.EnterArray( "Nodes" );

    for( auto & nodeParamMapping : m_paramNodes )
    {
        if( auto node = nodeParamMapping.second.lock() )
        {
            response.EnterChild( "NodeMapping" );

            response.SetAttribute( "NodeName", model::ModelState::GetInstance().BuildIndexPath( node.get() ) );
            response.SetAttribute( "ParamName", nodeParamMapping.first.GetParameter().GetName() );

            response.ExitChild();   // NodeMapping
        }
    }

    response.ExitChild();   // Nodes
}

}   // nodelogic
}	// bv
