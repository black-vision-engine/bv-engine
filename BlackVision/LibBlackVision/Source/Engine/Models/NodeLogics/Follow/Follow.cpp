#include "stdafx.h"
#include "Follow.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/ModelState.h"
#include "Engine/Models/BoundingVolume.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"


DEFINE_ENUM_PARAMETER_CREATOR( bv::nodelogic::Follow::FollowingMode );
DEFINE_ENUM_PARAMETER_CREATOR( bv::nodelogic::Follow::BBAlignementX );
DEFINE_ENUM_PARAMETER_CREATOR( bv::nodelogic::Follow::BBAlignementY );
DEFINE_ENUM_PARAMETER_CREATOR( bv::nodelogic::Follow::BBAlignementZ );





namespace bv {
namespace nodelogic
{



const std::string       Follow::m_type = "Follow";

//const std::string       Follow::ACTION::ACTION_NAME        = "ActionName";

const std::string       Follow::PARAMETERS::OFFSET_X = "OffsetX";
const std::string       Follow::PARAMETERS::OFFSET_Y = "OffsetY";
const std::string       Follow::PARAMETERS::OFFSET_Z = "OffsetZ";

const std::string       Follow::PARAMETERS::TARGET_ALIGN_X = "AlignX";
const std::string       Follow::PARAMETERS::TARGET_ALIGN_Y = "AlignY";
const std::string       Follow::PARAMETERS::TARGET_ALIGN_Z = "AlignZ";

const std::string       Follow::PARAMETERS::FOLLOWER_ALIGN_X = "FollowerAlignX";
const std::string       Follow::PARAMETERS::FOLLOWER_ALIGN_Y = "FollowerAlignY";
const std::string       Follow::PARAMETERS::FOLLOWER_ALIGN_Z = "FollowerAlignZ";

const std::string       Follow::PARAMETERS::FOLLOW_X = "FollowX";
const std::string       Follow::PARAMETERS::FOLLOW_Y = "FollowY";
const std::string       Follow::PARAMETERS::FOLLOW_Z = "FollowZ";

const std::string       Follow::PARAMETERS::FOLLOWING_MODE = "FollowingMode";
const std::string       Follow::PARAMETERS::FOLLOWING_NODE_PATH = "FollowingNodePath";

const std::string       Follow::PARAMETERS::TARGET_BOX_RECURSIVE = "TargetBoxRecursive";
const std::string       Follow::PARAMETERS::FOLLOWER_BOX_RECURSIVE = "FollowerBoxRecursive";



// ***********************
//
const std::string &     Follow::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     Follow::GetType             () const
{
    return Type();
}

// ***********************
//
Follow::Follow             ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( PARAMETERS::OFFSET_X, 0.0f, true, false );
    h.AddSimpleParam( PARAMETERS::OFFSET_Y, 0.0f, true, false );
    h.AddSimpleParam( PARAMETERS::OFFSET_Z, 0.0f, true, false );

    h.AddSimpleParam( PARAMETERS::FOLLOWING_NODE_PATH, std::string( "" ), true, false );
    h.AddEnumParam( PARAMETERS::FOLLOWING_MODE, FollowingMode::Previous, false, false );

    h.AddEnumParam( PARAMETERS::TARGET_ALIGN_X, BBAlignementX::CenterX, false, false );
    h.AddEnumParam( PARAMETERS::TARGET_ALIGN_Y, BBAlignementY::CenterY, false, false );
    h.AddEnumParam( PARAMETERS::TARGET_ALIGN_Z, BBAlignementZ::CenterZ, false, false );

    h.AddEnumParam( PARAMETERS::FOLLOWER_ALIGN_X, BBAlignementX::CenterX, false, false );
    h.AddEnumParam( PARAMETERS::FOLLOWER_ALIGN_Y, BBAlignementY::CenterY, false, false );
    h.AddEnumParam( PARAMETERS::FOLLOWER_ALIGN_Z, BBAlignementZ::CenterZ, false, false );

    h.AddSimpleParam( PARAMETERS::FOLLOW_X, true, true, false );
    h.AddSimpleParam( PARAMETERS::FOLLOW_Y, true, true, false );
    h.AddSimpleParam( PARAMETERS::FOLLOW_Z, true, true, false );

    h.AddSimpleParam( PARAMETERS::TARGET_BOX_RECURSIVE, true, true, false );
    h.AddSimpleParam( PARAMETERS::FOLLOWER_BOX_RECURSIVE, true, true, false );

    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );

    m_offsetX = QueryTypedValue< ValueFloatPtr >( m_paramValModel->GetValue( PARAMETERS::OFFSET_X ) );
    m_offsetY = QueryTypedValue< ValueFloatPtr >( m_paramValModel->GetValue( PARAMETERS::OFFSET_Y ) );
    m_offsetZ = QueryTypedValue< ValueFloatPtr >( m_paramValModel->GetValue( PARAMETERS::OFFSET_Z ) );

    m_followX = QueryTypedValue< ValueBoolPtr >( m_paramValModel->GetValue( PARAMETERS::FOLLOW_X ) );
    m_followY = QueryTypedValue< ValueBoolPtr >( m_paramValModel->GetValue( PARAMETERS::FOLLOW_Y ) );
    m_followZ = QueryTypedValue< ValueBoolPtr >( m_paramValModel->GetValue( PARAMETERS::FOLLOW_Z ) );

    m_alignX = QueryTypedEnum< BBAlignementX >( PARAMETERS::TARGET_ALIGN_X );
    m_alignY = QueryTypedEnum< BBAlignementY >( PARAMETERS::TARGET_ALIGN_Y );
    m_alignZ = QueryTypedEnum< BBAlignementZ >( PARAMETERS::TARGET_ALIGN_Z );

    m_followerAlignX = QueryTypedEnum< BBAlignementX >( PARAMETERS::FOLLOWER_ALIGN_X );
    m_followerAlignY = QueryTypedEnum< BBAlignementY >( PARAMETERS::FOLLOWER_ALIGN_Y );
    m_followerAlignZ = QueryTypedEnum< BBAlignementZ >( PARAMETERS::FOLLOWER_ALIGN_Z );

    m_followingMode = QueryTypedEnum< FollowingMode >( PARAMETERS::FOLLOWING_MODE );
    m_nodePath = QueryTypedValue< ValueStringPtr >( m_paramValModel->GetValue( PARAMETERS::FOLLOWING_NODE_PATH ) );

    m_targetBoxRecursive = QueryTypedValue< ValueBoolPtr >( m_paramValModel->GetValue( PARAMETERS::TARGET_BOX_RECURSIVE ) );
    m_followerBoxRecursive = QueryTypedValue< ValueBoolPtr >( m_paramValModel->GetValue( PARAMETERS::FOLLOWER_BOX_RECURSIVE ) );
}

// ***********************
//
Follow::~Follow()
{}

// ***********************
//
void        Follow::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &Follow::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void        Follow::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &Follow::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void                        Follow::PreNodeUpdate   ( TimeType t )
{
    NodeLogicBase::Update( t );

    auto node = GetObservedNode();
    auto followingNode = std::static_pointer_cast< const model::BasicNode >( m_parentNode.lock() );
    
    glm::vec3 followedPoint = GetBBPoint( node, m_alignX, m_alignY, m_alignZ, m_targetBoxRecursive->GetValue() );
    glm::mat4 transformBox = GetBBTransform( node );
    followedPoint = glm::vec3( transformBox * glm::vec4( followedPoint, 1.0f ) );

    glm::vec3 followingPoint = GetBBPoint( followingNode, m_followerAlignX, m_followerAlignY, m_followerAlignZ, m_followerBoxRecursive->GetValue() );
    glm::mat4 followerTransform = GetBBTransform( followingNode );
    followingPoint = glm::vec3( followerTransform * glm::vec4( followingPoint, 1.0f ) );
    
    glm::vec3 curTranslation = followingNode->GetFinalizePlugin()->GetParamTransform()->GetTransform().GetTranslation( 0.0f );
    followingPoint = followingPoint - curTranslation;

    glm::vec3 translation = followedPoint - followingPoint;
    
    ApplyTranslation( translation );
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        Follow::Serialize       ( ISerializer & ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
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
FollowPtr              Follow::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< Follow >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        Follow::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

//    if( action == Follow::ACTION::ACTION_NAME )
//    {
//        return false
//    }

    return false;
}

// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        Follow::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;

    if( auto parentNode = m_parentNode.lock() )
    {

    }
}

// ========================================================================= //
// Follow logic functions
// ========================================================================= //

// ***********************
//
model::BasicNodeConstPtr    Follow::GetObservedNode     ()
{
    if( auto node = m_parentNode.lock() )
    {
        auto parentParent = static_cast< const model::BasicNode * >( model::ModelState::GetInstance().QueryNodeParent( node.get() ) );

        FollowingMode mode = (FollowingMode)m_followingMode->Evaluate();

        if( mode == FollowingMode::FirstInSubtree )
        {
            auto brother = static_cast< const model::BasicNode * >( parentParent->GetChild( 0 ) );
            if( brother == node.get() )
                return nullptr;

            return brother->shared_from_this();
        }
        else if( mode == FollowingMode::Path )
        {
            auto foundNode = const_cast< bv::model::BasicNode * >( parentParent )->GetNode( m_nodePath->GetValue() );
            return std::static_pointer_cast< bv::model::BasicNode >( foundNode );
        }
        else if( mode == FollowingMode::Previous )
        {
            for( unsigned int i = 1; i < parentParent->GetNumChildren(); ++i )
            {
                if( parentParent->GetChild( i ) == node.get() )
                {
                    auto brother = static_cast< const model::BasicNode * >( parentParent->GetChild( i - 1 ) );
                    return brother->shared_from_this();
                }
            }
        }
    }

    return nullptr;
}

// ***********************
//
glm::vec3                   Follow::GetBBPoint          ( model::BasicNodeConstPtr & node, const BBAlignementXParamPtr & alignXParam, const BBAlignementYParamPtr & alignYParam, const BBAlignementZParamPtr & alignZParam, bool recursiveBox )
{
    if( node != nullptr )
    {
        mathematics::Box bb;

        if( recursiveBox )
        {
            bb = node->GetBoundingBoxRecursive();
        }
        else
        {
            auto boundingVolume = node->GetBoundingVolume();
            bb = *boundingVolume->GetBoundingBox();
        }

        BBAlignementX alignX = static_cast< BBAlignementX >( alignXParam->Evaluate() );
        BBAlignementY alignY = static_cast< BBAlignementY >( alignYParam->Evaluate() );
        BBAlignementZ alignZ = static_cast< BBAlignementZ >( alignZParam->Evaluate() );

        glm::vec3 BBPoint;

        switch( alignX )
        {
            case BBAlignementX::Left:
                BBPoint.x = bb.xmin;
                break;
            case BBAlignementX::Right:
                BBPoint.x = bb.xmax;
                break;
            case BBAlignementX::CenterX:
                BBPoint.x = ( bb.xmax + bb.xmin ) / 2;
                break;
        }

        switch( alignY )
        {
            case BBAlignementY::Bottom:
                BBPoint.y = bb.ymin;
                break;
            case BBAlignementY::Top:
                BBPoint.y = bb.ymax;
                break;
            case BBAlignementY::CenterY:
                BBPoint.y = ( bb.ymax + bb.ymin ) / 2;
                break;
        }

        switch( alignZ )
        {
            case BBAlignementZ::Back:
                BBPoint.z = bb.zmin;
                break;
            case BBAlignementZ::Front:
                BBPoint.z = bb.zmax;
                break;
            case BBAlignementZ::CenterZ:
                BBPoint.z = ( bb.zmax + bb.zmin ) / 2;
                break;
        }

        return BBPoint;
    }
    return glm::vec3( 0.0, 0.0, 0.0 );
}

// ***********************
//
glm::mat4                   Follow::GetBBTransform      ( model::BasicNodeConstPtr & node )
{
    if( node != nullptr )
        return node->GetFinalizePlugin()->GetTransformChannel()->GetTransformValue()->GetValue();
    else
        return glm::mat4( 1.0 );
}

// ***********************
//
void                        Follow::ApplyTranslation    ( glm::vec3 & transform )
{
    if( auto node = m_parentNode.lock() )
    {
        transform.x += m_offsetX->GetValue();
        transform.y += m_offsetY->GetValue();
        transform.z += m_offsetZ->GetValue();

        auto transformParam = node->GetFinalizePlugin()->GetParamTransform();
        glm::vec3 curTranslation = transformParam->GetTransform().GetTranslation( 0.0 );

        transform.x = m_followX->GetValue() ? transform.x : curTranslation.x;
        transform.y = m_followY->GetValue() ? transform.y : curTranslation.y;
        transform.z = m_followZ->GetValue() ? transform.z : curTranslation.z;

        model::SetParameterTranslation( transformParam, 0.0, transform );
    }
}


}   // nodelogic
}	// bv
