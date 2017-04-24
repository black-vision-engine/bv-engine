#include "stdafx.h"
#include "Cloner.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"
#include "Engine/Events/InnerEvents/Nodes/NodeRemovedEvent.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Models/NodeLogics/NodeLogicHelper.h"

#include "Engine/Models/ModelState.h"
#include "Engine/Models/BVProject.h"
#include "Engine/Models/BVProjectEditor.h"

namespace bv
{
namespace nodelogic
{

static UInt32 HARD_CLONER_NODES_LIMIT = 3000; // FIXME: Make this property setable from config.

const std::string       Cloner::m_type = "Cloner";

const std::string       Cloner::ACTION::REGENERATE          = "Regenerate";
const std::string       Cloner::ACTION::REMOVE_ONLY_EXCESS  = "RemoveOnlyExcess";
const std::string       Cloner::ACTION::REMOVE_ALL_CLONES   = "RemoveAllClones";

const std::string       Cloner::PARAMETERS::N_ROWS  = "numRows";
const std::string       Cloner::PARAMETERS::N_COLS  = "numCols";
const std::string       Cloner::PARAMETERS::DELTA   = "delta";
const std::string       Cloner::PARAMETERS::RENAME_SUBTREE  = "renameSubTree";
const std::string       Cloner::PARAMETERS::REMOVE_EXCESS   = "removeExcess";
const std::string       Cloner::PARAMETERS::PLANE_TYPE      = "planeType";

// ***********************
//
const std::string &     Cloner::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &     Cloner::GetType             () const
{
    return Type();
}

// ***********************
//
Cloner::Cloner             ( bv::model::BasicNodeWeakPtr parent, bv::model::ITimeEvaluatorPtr timeEvaluator )
    : m_parentNode( parent )
    , m_updatePositionsNeeded( true )
    , m_updateClonesNeeded( true )
{
    model::ModelHelper h( timeEvaluator );
    h.SetOrCreatePluginModel();

    h.AddSimpleParam( PARAMETERS::N_ROWS, 1, true, true );
    h.AddSimpleParam( PARAMETERS::N_COLS, 1, true, true );
    h.AddSimpleParam( PARAMETERS::DELTA, glm::vec3( 0.f, 0.f, 0.f ), true, true );
    h.AddSimpleParam( PARAMETERS::RENAME_SUBTREE, false, true, true );
    h.AddSimpleParam( PARAMETERS::REMOVE_EXCESS, false, true, true );
    h.AddEnumParam< ClonerPlaneType >( PARAMETERS::PLANE_TYPE, ClonerPlaneType::CPT_XY, true, true );

    m_paramValModel = std::static_pointer_cast< model::DefaultParamValModel >( h.GetModel()->GetPluginModel() );

    m_numRows = model::GetValueParamState< Int32 >( m_paramValModel.get(), PARAMETERS::N_ROWS );
    m_numCols = model::GetValueParamState< Int32 >( m_paramValModel.get(), PARAMETERS::N_COLS );
    m_delta = model::GetValueParamState< glm::vec3 >( m_paramValModel.get(), PARAMETERS::DELTA );
    m_renameSubtree = model::GetValueParamState< bool >( m_paramValModel.get(), PARAMETERS::RENAME_SUBTREE );
    m_removeExcees = model::GetValueParamState< bool >( m_paramValModel.get(), PARAMETERS::REMOVE_EXCESS );
    m_planeType = model::GetValueParamState< ClonerPlaneType >( m_paramValModel.get(), PARAMETERS::PLANE_TYPE );
}

// ***********************
//
Cloner::~Cloner()
{}


// ***********************
//
void        Cloner::Initialize        ()
{
    GetDefaultEventManager().AddListener( fastdelegate::MakeDelegate( this, &Cloner::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void        Cloner::Deinitialize      ()
{
    GetDefaultEventManager().RemoveListener( fastdelegate::MakeDelegate( this, &Cloner::NodeRemovedHandler ), NodeRemovedEvent::Type() );
}

// ***********************
//
void                        Cloner::Update			( TimeType t )
{
    NodeLogicBase::Update( t );

    if( ParameterChanged( PARAMETERS::N_ROWS ) ||
        ParameterChanged( PARAMETERS::N_COLS ) )
    {
        m_updatePositionsNeeded = true;
        m_updateClonesNeeded = true;
    }

    if( ParameterChanged( PARAMETERS::DELTA ) ||
        ParameterChanged( PARAMETERS::PLANE_TYPE ) )
        m_updatePositionsNeeded = true;

    UpdateClones();
    UpdatePositions();
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                        Cloner::Serialize       ( ISerializer & ser ) const
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
ClonerPtr              Cloner::Create          ( const IDeserializer & deser, bv::model::BasicNodeWeakPtr parentNode )
{
    auto timeline = SerializationHelper::GetDefaultTimeline( deser );
    auto newLogic = std::make_shared< Cloner >( parentNode, timeline );

    newLogic->Deserialize( deser );

    return newLogic;
}

// ========================================================================= //
// Commands handling
// ========================================================================= //

// ***********************
//
bool                        Cloner::HandleEvent     ( IDeserializer & eventDeser, ISerializer & /*response*/, BVProjectEditor * /*editor*/ )
{
    std::string action = eventDeser.GetAttribute( "Action" );

    if( action == Cloner::ACTION::REGENERATE )
        Regenerate();
    else if( action == Cloner::ACTION::REMOVE_ONLY_EXCESS )
        RemoveExcessNodes();
    else if( action == Cloner::ACTION::REMOVE_ALL_CLONES )
        RemoveClones();

    return false;
}

// ========================================================================= //
// Handling removing of nodes
// ========================================================================= //

// ***********************
//
void                        Cloner::NodeRemovedHandler  ( IEventPtr evt )
{
    if( evt->GetEventType() != NodeRemovedEvent::Type() )
        return;

    if( auto parentNode = m_parentNode.lock() )
    {
        
    }
}

// ***********************
//
void                        Cloner::UpdateClones        ()
{
    if( m_updateClonesNeeded )
    {
        if( auto parentNode = m_parentNode.lock() )
        {
            auto missingNum = m_numCols.GetValue() * m_numRows.GetValue() - ( Int32 )parentNode->GetNumChildren();

            if( missingNum > 0 )
                CloneNode( missingNum );
        }

        m_updateClonesNeeded = false;
    }
}

// ***********************
//
void                        Cloner::UpdatePositions     ()
{
    if( m_updatePositionsNeeded )
    {
        if( auto parentNode = m_parentNode.lock() )
        {
            auto & modelState = model::ModelState::GetInstance();

            auto sceneName = modelState.QueryNodeScene( parentNode.get() )->GetName();

            auto scene = modelState.GetBVProject()->GetModelScene( sceneName );

            auto projectEditor = modelState.GetBVProject()->GetProjectEditor();

            if( parentNode->GetNumChildren() > 0 )
            {
                auto firstChild = parentNode->GetChild( 0 );

                auto paramTransformFirst = firstChild->GetFinalizePlugin()->GetParamTransform();

                glm::vec3 translation = glm::vec3( 0.f );

                if( paramTransformFirst )
                    translation = paramTransformFirst->GetTransform().GetTranslation( 0.f );

                auto numCols = m_numCols.GetValue();
                auto numRows = m_numRows.GetValue();
                auto delta = m_delta.GetValue();
                auto renameSubtree = m_renameSubtree.GetValue();
                auto removeExcees = m_removeExcees.GetValue();

                auto firstChildName = firstChild->GetName();

                Int32 i = 1;
                auto numChindren = ( Int32 ) parentNode->GetNumChildren();
                auto numVisible = std::min( numChindren, numRows * numCols );

                for( ;i < numVisible; ++i )
                {
                    auto ch = parentNode->GetChild( i );
                    ch->SetVisible( true );

                    auto paramTransform = ch->GetFinalizePlugin()->GetParamTransform();

                    auto r = i / numCols;
                    auto c = i % numCols;

                    if( renameSubtree )
                        ch->SetName( firstChildName + "_" + std::to_string( r ) + "_" + std::to_string( c ) );

                    auto d = Transform2Plane( delta, m_planeType.GetValue() );

                    if( paramTransform )
                        paramTransform->SetTranslation( translation + glm::vec3( d.x * c, d.y * r, d.z * c ), 0.f );
                }

                for( ; i < numChindren; ++i )
                {
                    if( !removeExcees )
                        parentNode->GetChild( i )->SetVisible( false );
                    else
                        projectEditor->DeleteChildNode( scene, parentNode, parentNode->GetChild( i ), false );
                }

            }
        }

        m_updatePositionsNeeded = false;
    }
}

// ***********************
//
void                        Cloner::CloneNode           ( UInt32 clonesNum ) const
{
    if( auto parentNode = m_parentNode.lock() )
    {
        auto & modelState = model::ModelState::GetInstance();

        auto sceneName = modelState.QueryNodeScene( parentNode.get() )->GetName();

        auto scene = modelState.GetBVProject()->GetModelScene( sceneName );

        auto projectEditor = modelState.GetBVProject()->GetProjectEditor();

        auto numChildren = parentNode->GetNumChildren();

        if( numChildren + clonesNum > HARD_CLONER_NODES_LIMIT ) // Applying limit for number for cloned nodes.
            clonesNum = HARD_CLONER_NODES_LIMIT - numChildren;

        if( numChildren > 0 )
        {
            auto firstChild = parentNode->GetChild( 0 );

            for( UInt32 i = 0; i < clonesNum; ++i )
                projectEditor->AddNodeCopy( scene, parentNode, scene, firstChild, false );
        }
    }
}

// ***********************
//
glm::vec3                   Cloner::Transform2Plane     ( const glm::vec3 & v, ClonerPlaneType plane ) const
{
    switch( plane )
    {
    case ClonerPlaneType::CPT_XZ:
        return glm::vec3( v.x, v.z, v.y );
        break;
    case ClonerPlaneType::CPT_YZ:
        return glm::vec3( v.z, v.y, v.x );
        break;
    default:
        return v;
        break;
    }
}

// ***********************
//
void                        Cloner::RemoveClones            ()
{
    if( auto parentNode = m_parentNode.lock() )
    {
        auto & modelState = model::ModelState::GetInstance();

        auto sceneName = modelState.QueryNodeScene( parentNode.get() )->GetName();

        auto scene = modelState.GetBVProject()->GetModelScene( sceneName );

        auto projectEditor = modelState.GetBVProject()->GetProjectEditor();

        while( parentNode->GetNumChildren() > 1 )
            projectEditor->DeleteChildNode( scene, parentNode, parentNode->GetChild( 1 ), false );
    }
}

// ***********************
//
void                        Cloner::RemoveExcessNodes       ()
{
    if( auto parentNode = m_parentNode.lock() )
    {
        auto & modelState = model::ModelState::GetInstance();

        auto sceneName = modelState.QueryNodeScene( parentNode.get() )->GetName();

        auto scene = modelState.GetBVProject()->GetModelScene( sceneName );

        auto projectEditor = modelState.GetBVProject()->GetProjectEditor();

        auto numClones = m_numCols.GetValue() * m_numCols.GetValue();

        while( numClones > 0 && ( Int32 ) parentNode->GetNumChildren() > numClones )
            projectEditor->DeleteChildNode( scene, parentNode, parentNode->GetChild( parentNode->GetNumChildren() - 1 ), false );
    }
}

// ***********************
//
void                        Cloner::Regenerate              ()
{
    RemoveClones();

    m_updateClonesNeeded = true;
    m_updatePositionsNeeded = true;
}


} // nodelogic
} // bv
