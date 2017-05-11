#include "stdafx.h"
#include "NodeVisibilityAnimation.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeMovedEvent.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Models/BVProjectEditor.h"
#include "Engine/Models/ModelSceneEditor.h"
#include "Engine/Models/SceneModel.h"

#include "Engine/Events/EventHandlerHelpers.h"

namespace bv
{
namespace nodelogic
{

const std::string       NodeVisibilityAnimation::m_type = "NodeVisibilityAnimation";

const std::string       NodeVisibilityAnimation::ACTION::REGISTER_NODE_VISIBILITY_PARAM = "RegisterNodeVisilibityParam";

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
bool                        NodeVisibilityAnimation::HandleEvent     ( IDeserializer & eventDeser, ISerializer & response, BVProjectEditor * editor )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == NodeVisibilityAnimation::ACTION::REGISTER_NODE_VISIBILITY_PARAM )
    {
        auto nodePath = eventDeser.GetAttribute( "NodePath" );
        auto sceneName = eventDeser.GetAttribute( "SceneName" );
        if( !nodePath.empty() && !sceneName.empty() )
        {
            if( RegisterNodeVisibilityParam( sceneName, nodePath, editor ) )
                return true;
            else
            {
                response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only 'Replicate' command." );
                return false;
            }
        }
    }
    else
        response.SetAttribute( ERROR_INFO_STRING, "Unknown command. This logic supports only ' " + NodeVisibilityAnimation::ACTION::REGISTER_NODE_VISIBILITY_PARAM + " ' command." );

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
        
        std::string paramName = "";

        for( auto it = m_paramNodes.begin(); it != m_paramNodes.end(); ++it )
        {
            auto node = ( *it ).second.lock();
            if( node == typedEvent->RemovedNode )
            {
                paramName = ( *it ).first.GetParameter().GetName();
                it = m_paramNodes.erase( it );
            }
        }
        
        if( !paramName.empty() )
            m_paramValModel->RemoveParamVal( paramName );
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

    if( auto parentNode = m_parentNode.lock() )
    {
        auto typedEvent = QueryTypedEvent< NodeMovedEvent >( evt );

        for( auto it = m_paramNodes.begin(); it != m_paramNodes.end(); ++it )
        {
            auto node = ( *it ).second.lock();
            if( node == typedEvent->Node )
            {
                //( *it ).first.GetParameter().
            }
        }
    }
}

// ***********************
//
bool                        NodeVisibilityAnimation::RegisterNodeVisibilityParam( const std::string & sceneName, const std::string & nodePath, BVProjectEditor * editor )
{
    auto modelScene = editor->GetModelScene( sceneName );

    if( modelScene )
    {
        auto node = modelScene->GetModelSceneEditor()->GetNode( nodePath );
        
        if( node )
        {
            model::ModelHelper h( m_defaultTimeEvaluator );
            h.SetOrCreatePluginModel();

            auto paramName = NODE_VISIBILITY_PARAM_NAME_PREFIX + nodePath;

            h.AddSimpleParam< bool >( paramName, node->IsVisible(), true, true );

            m_paramNodes.push_back( std::make_pair( model::GetValueParamState< bool >( m_paramValModel.get(), paramName ), node ) );

            return true;
        }
    }

    return false;
}

}   // nodelogic
}	// bv
