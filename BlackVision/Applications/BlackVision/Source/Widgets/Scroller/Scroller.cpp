#include "Scroller.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "ScrollerEvents.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Tools/StringHeplers.h"
#include "EndUserAPI/EventHandlers/EventHandlerHelpers.h"

#include "Mathematics/glm_inc.h"

#include "ProjectManager.h"
#include "Engine/Models/BVProjectEditor.h"

#include <algorithm>
#include "System/Time.h"

namespace bv {
    
namespace SerializationHelper {


std::pair< bv::nodelogic::Scroller::ScrollDirection, const char* > ScrollDirectionMapping[] = 
    { std::make_pair( bv::nodelogic::Scroller::ScrollDirection::SD_Down, "ScrollDown" )
    , std::make_pair( bv::nodelogic::Scroller::ScrollDirection::SD_Up, "ScrollUp" )
    , std::make_pair( bv::nodelogic::Scroller::ScrollDirection::SD_Left, "ScrollLeft" )
    , std::make_pair( bv::nodelogic::Scroller::ScrollDirection::SD_Right, "ScrollRight" )
    , std::make_pair( bv::nodelogic::Scroller::ScrollDirection::SD_Total, "" )      // default
};

template<> bv::nodelogic::Scroller::ScrollDirection String2T        ( const std::string & s, const bv::nodelogic::Scroller::ScrollDirection & defaultVal )    { return String2Enum( ScrollDirectionMapping, s, defaultVal ); }
template<> std::string                              T2String        ( const bv::nodelogic::Scroller::ScrollDirection & t )                                    { return Enum2String( ScrollDirectionMapping, t ); }
    

std::pair< bv::nodelogic::Scroller::OffscreenNodeBehavior, const char* > OffscreenNodeBehaviorMapping[] = 
{   std::make_pair( bv::nodelogic::Scroller::OffscreenNodeBehavior::ONB_Looping, "Looping" )
    , std::make_pair( bv::nodelogic::Scroller::OffscreenNodeBehavior::ONB_DeleteNode, "DeleteNode" )
    , std::make_pair( bv::nodelogic::Scroller::OffscreenNodeBehavior::ONB_SetNonActive, "SetNonActive" )
    , std::make_pair( bv::nodelogic::Scroller::OffscreenNodeBehavior::ONB_Total, "" )      // default
};

template<> bv::nodelogic::Scroller::OffscreenNodeBehavior   String2T        ( const std::string & s, const bv::nodelogic::Scroller::OffscreenNodeBehavior & defaultVal )    { return String2Enum( OffscreenNodeBehaviorMapping, s, defaultVal ); }
template<> std::string                                      T2String        ( const bv::nodelogic::Scroller::OffscreenNodeBehavior & t )                                    { return Enum2String( OffscreenNodeBehaviorMapping, t ); }
    

// ***********************
//
mathematics::RectPtr        CreateRect      ( const IDeserializer & deser )
{
    mathematics::RectPtr rect = std::make_shared<mathematics::Rect>();

    if( deser.EnterChild( "view" ) )
    {
        bool empty = SerializationHelper::String2T( deser.GetAttribute( "empty" ), true );
        if( !empty )
        {
            rect->xmin = SerializationHelper::String2T( deser.GetAttribute( "xmin" ), 0.0f );
            rect->xmax = SerializationHelper::String2T( deser.GetAttribute( "xmax" ), 0.0f );
            rect->ymax = SerializationHelper::String2T( deser.GetAttribute( "ymax" ), 0.0f );
            rect->ymin = SerializationHelper::String2T( deser.GetAttribute( "ymin" ), 0.0f );
        }
        deser.ExitChild(); // view
    }

    return rect;
}

// ***********************
//
void                        SerializeRect       ( ISerializer & ser, mathematics::RectPtr view )
{
    ser.EnterChild( "view" );
        ser.SetAttribute( "empty", SerializationHelper::T2String( view->m_empty ) );
        if( !view->m_empty )
        {
            ser.SetAttribute( "xmin", SerializationHelper::T2String( view->xmin ) );
            ser.SetAttribute( "xmax", SerializationHelper::T2String( view->xmax ) );
            ser.SetAttribute( "ymin", SerializationHelper::T2String( view->ymin ) );
            ser.SetAttribute( "ymax", SerializationHelper::T2String( view->ymax ) );
        }
    ser.ExitChild(); // view
}

}   // SerializationHelper
    
namespace nodelogic {


const std::string   Scroller::m_type = "scroller";

// ***********************
//
const std::string &  Scroller::Type            ()
{
    return m_type;
}

// ***********************
//
const std::string &  Scroller::GetType             () const
{
    return Type();
}


namespace
{
// ***********************
//
bv::model::BasicNodePtr         GetNode     ( bv::model::BasicNode * parent, Int32 nodeIdx )
{
    return std::static_pointer_cast<bv::model::BasicNode>( parent->GetChild( nodeIdx ) );
}

// ***********************
//
bv::model::BasicNodePtr         GetNode     ( bv::model::BasicNode * parent, const std::string& nodeName )
{
    return std::static_pointer_cast<bv::model::BasicNode>( parent->GetNode( nodeName ) );
}

// ***********************
//
glm::vec3       ScrollerShiftToVec   ( Scroller::ScrollDirection crawlDirection )
{
    glm::vec3 shiftDirection;
    if( crawlDirection == Scroller::ScrollDirection::SD_Down || crawlDirection == Scroller::ScrollDirection::SD_Up )
        shiftDirection = glm::vec3( 0.0, 1.0, 0.0 );
    else if( crawlDirection == Scroller::ScrollDirection::SD_Right || crawlDirection == Scroller::ScrollDirection::SD_Left )
        shiftDirection = glm::vec3( 1.0, 0.0, 0.0 );
    else
        shiftDirection = glm::vec3( 0.0, 0.0, 0.0 );

    return shiftDirection;
}

} // anonymous


// *******************************
//
ScrollerPtr	Scroller::Create				( bv::model::BasicNodePtr parent, const mathematics::RectPtr & view )
{
	return std::make_shared< Scroller >( parent, view );
}

// *******************************
//
Scroller::Scroller						( bv::model::BasicNodePtr parent, const mathematics::RectPtr & view )
	: m_parentNode( parent )
    , m_editor( nullptr )
	, m_isFinalized( false )
	, m_view( view )
	, m_started( false )
	, m_currTime( 0 )
    , m_smoothTime( 3000 )
    , m_smoothStart( false )
    , m_smoothPause( false )
	, m_speed( 0.f )
	, m_interspace( 0.0f )
    , m_paused( false )
    , m_scrollDirection( ScrollDirection::SD_Left )
    , m_enableEvents( false )
    , m_lowBufferMultiplier( 3.5 )
    , m_offscreenNodeBehavior( OffscreenNodeBehavior::ONB_SetNonActive )
{}


// ========================================================================= //
// Node managing
// ========================================================================= //


// *******************************
//
void		Scroller::AddNext			( bv::model::BasicNodePtr node )
{
    if( !m_started )
	{
		m_parentNode->AddChildToModelOnly( node );
		m_nodesStates.Add( node.get() );
	}
	else
		assert(!"Scroller: Cannot add node while scrolling!");
}

// ***********************
//
bool		Scroller::AddNext            ( Int32 nodeIdx )
{
    auto newNode = GetNode( m_parentNode.get(), nodeIdx );
    return AddNode( newNode );
}

// ***********************
//
bool		Scroller::AddNext				( const std::string& childNodeName )
{
    auto newNode = GetNode( m_parentNode.get(), childNodeName );
    return AddNode( newNode );
}

// ***********************
//
bool        Scroller::AddNode             ( bv::model::BasicNodePtr node )
{
    bool alreadyExists = m_nodesStates.Exist( node.get() );
    
    assert( !alreadyExists );
    if( alreadyExists )
        return false;

	m_nodesStates.Add( node.get() );

    if( m_started )
	{
        ShiftNodeToEnd( node.get() );
	}

    return true;
}

// ***********************
//
Float32     Scroller::InitialShift        ( model::BasicNode * node )
{
    Float32 shift;
    if( m_scrollDirection == Scroller::ScrollDirection::SD_Down )
    {
        shift = m_view->ymax + node->GetAABB().Height() / 2.0f + m_interspace;
    }
    else if( m_scrollDirection == Scroller::ScrollDirection::SD_Up )
    {
        shift = m_view->ymin - node->GetAABB().Height() / 2.0f - m_interspace;
    }
    else if( m_scrollDirection == Scroller::ScrollDirection::SD_Right )
    {
        shift = m_view->xmin - node->GetAABB().Width() - m_interspace;
    }
    else if( m_scrollDirection == Scroller::ScrollDirection::SD_Left )
    {
        shift = m_view->xmax + m_interspace;
    }
    else
    {
        shift = 0.0f;
    }

    return shift;
}

// ***********************
//
Float32     Scroller::ShiftStep           ( model::BasicNode * prevNode, model::BasicNode * curNode )
{
    Float32 shift;
    if( m_scrollDirection == Scroller::ScrollDirection::SD_Down )
    {
        shift = ( prevNode->GetAABB().Height() + curNode->GetAABB().Height() ) / 2.0f + m_interspace;
    }
    else if( m_scrollDirection == Scroller::ScrollDirection::SD_Up )
    {
        shift = ( prevNode->GetAABB().Height() + curNode->GetAABB().Height() ) / 2.0f - m_interspace;
    }
    else if( m_scrollDirection == Scroller::ScrollDirection::SD_Right )
    {
        shift = -curNode->GetAABB().Width() - m_interspace;
    }
    else if( m_scrollDirection == Scroller::ScrollDirection::SD_Left )
    {
        shift = prevNode->GetAABB().Width() + m_interspace;
    }
    else
    {
        shift = 0.0f;
    }

    return shift;
}

// ***********************
//
Float32     Scroller::SignedShift         ( Float32 shift )
{
    if( m_scrollDirection == Scroller::ScrollDirection::SD_Down || m_scrollDirection == Scroller::ScrollDirection::SD_Left )
        return -shift;
    return shift;
}

// ***********************
//
Float32     Scroller::Smooth              ( UInt64 time, Float32 shift )
{
    if( m_smoothStart || m_smoothPause )
    {
        double timeDiff = (double)( time - m_smoothStartTime );
        double smoothTime = (double)m_smoothTime;

        auto interpolate = timeDiff / smoothTime;
        Float32 newShift = static_cast< Float32 >( shift * sin( glm::half_pi<double>() * interpolate ) );

        // End of smooth start
        if( m_smoothStart && timeDiff >= smoothTime )
        {
            m_smoothStart = false;
        }

        // End of smooth pause
        if( m_smoothPause && timeDiff >= 2 * smoothTime )   // 2 * smoothTime - Check initial conditions to understand.
        {
            m_smoothPause = false;
            m_paused = true;
        }

        return newShift;
    }

    // No smooth applied
    return shift;
}

// *******************************
//
bool		Scroller::Finalize			()
{
	if( m_isFinalized )
		;//assert(!"Scroller: Already finalized!");
	else
	{
        glm::vec3 shiftDirection = ScrollerShiftToVec( m_scrollDirection );

		auto copy = m_nodesStates.m_nonActives;
		for( auto n : copy )	
			SetActiveNode( n );

		LayoutNodes();
		m_isFinalized = true;
	}

	return m_isFinalized;
}

// ***********************
//
bool        Scroller::Unfinalize          ()
{
    auto copy = m_nodesStates.m_actives;

    for( auto n : copy )	
		m_nodesStates.Deacivate( n );
    
    m_started = false;
    m_isFinalized = false;

    return true;
}


// *******************************
//
void		Scroller::LayoutNodes		()
{
	auto length = m_nodesStates.ActiveSize();
	if( length > 0 )
	{
		Float32 currShift = InitialShift( m_nodesStates.m_actives[ 0 ] );

		m_shifts[ m_nodesStates.m_actives[ 0 ] ] = currShift;

		for( SizeType i = 1; i < length; ++i )
		{
            currShift += ShiftStep( m_nodesStates.m_actives[ i - 1 ], m_nodesStates.m_actives[ i ] );

			m_shifts[ m_nodesStates.m_actives[ i ] ] = currShift;
		}

		UpdateTransforms();
	}
}

// *******************************
//
void		Scroller::Update				( TimeType )
{
    if( m_started )
	{
        auto t = Time::Now();

        if( !m_paused )
        {
		    auto shift = m_speed * ( ( t - m_currTime ) / 1000.f );
            shift = Smooth( t, shift );

		    if( shift > 0.f )
		    {
			    for( auto elem : m_shifts )
				    m_shifts[ elem.first ] += SignedShift( shift );

			    UpdateTransforms();

                if( CheckLowBuffer() )
                    NotifyLowBuffer();
		    }
        }

        m_currTime = t;
	}
}

// *******************************
//
void		Scroller::UpdateTransforms	()
{
    glm::vec3 shiftDirection = ScrollerShiftToVec( m_scrollDirection );

	for( auto elem : m_shifts )
	{
		if( IsActive( elem.first ) )
		{
			auto trPlugin = elem.first->GetPlugin( "transform" );
			if( trPlugin )
			{
				auto trParam = trPlugin->GetParameter( "simple_transform" );
				model::SetParameterTranslation( trParam, 0.0f, shiftDirection * elem.second );
			}
		}
	}

	auto copy = m_nodesStates.m_actives;

	for( auto n : copy )
		UpdateVisibility( n );
}

// *******************************
//
void		Scroller::UpdateVisibility	( bv::model::BasicNode * n )
{
	auto currVisibility = m_nodesStates.IsVisible( n );
	auto nAABB = n->GetAABB();
	bool newVisibility = nAABB.HasNonEmptyIntersection( *m_view );

	if( currVisibility != newVisibility )
	{
		if( newVisibility )
		{
			m_nodesStates.Visible( n );
			OnNotifyVisibilityChanged( n, newVisibility );
		}
		else if( IsActive( n ) )
		{
			m_nodesStates.NotVisible( n );
			OnNotifyVisibilityChanged( n, newVisibility );
		}
	}
}

// ***********************
//
void		Scroller::OnNotifyVisibilityChanged     ( bv::model::BasicNode * n, bool visibility )
{
    if( m_enableEvents )
    {
        NotifyVisibilityChanged( n, visibility );

        if( m_nodesStates.m_visibles.empty() )
            NotifyNoMoreNodes();
    }

    if( !visibility )
        OnNotifyNodeOffscreen( n );
}

// ***********************
//
void		Scroller::OnNotifyNodeOffscreen         ( bv::model::BasicNode * n )
{
    if( m_offscreenNodeBehavior == OffscreenNodeBehavior::ONB_Looping )
    {
        ShiftNodeToEnd( n );
    }
    else if( m_offscreenNodeBehavior == OffscreenNodeBehavior::ONB_SetNonActive )
    {
        // Function m_nodesStates.NotVisible is called in UpdateVisibility and has already
        // deactivated this node.
        //m_nodesStates.Deacivate( n );
    }
    else if( m_offscreenNodeBehavior == OffscreenNodeBehavior::ONB_DeleteNode )
    {
        m_nodesStates.Remove( n );
        m_editor->DeleteChildNode( m_editor->GetScene( m_sceneName ), m_parentNode, n->shared_from_this() );
    }
    else
        assert( false );
}

// *******************************
//
void		Scroller::NotifyVisibilityChanged       ( bv::model::BasicNode * n, bool visibility )
{
    assert( m_scrollerNodePath != "" );

    JsonSerializeObject ser;
    ser.SetAttribute( "ScrollerPath", m_scrollerNodePath );
    ser.SetAttribute( "NodeName", n->GetName() );

	if( visibility )
        ser.SetAttribute( "Event", "ItemOnScreen" );
	else
		ser.SetAttribute( "Event", "ItemOffScreen" );

    SendResponse( ser, SEND_BROADCAST_EVENT, 0 );
}

// *******************************
//
void		Scroller::NotifyNoMoreNodes ()
{
    assert( m_scrollerNodePath != "" );

    JsonSerializeObject ser;
    ser.SetAttribute( "ScrollerPath", m_scrollerNodePath );
    ser.SetAttribute( "Event", "AllItemsOffScreen" );

    SendResponse( ser, SEND_BROADCAST_EVENT, 0 );
}

// ***********************
//
void        Scroller::NotifyLowBuffer         ()
{
    assert( m_scrollerNodePath != "" );

    JsonSerializeObject ser;
    ser.SetAttribute( "ScrollerPath", m_scrollerNodePath );
    ser.SetAttribute( "Event", "LowBuffer" );

    SendResponse( ser, SEND_BROADCAST_EVENT, 0 );
}


// *******************************
//
void		Scroller::SetActiveNode		( bv::model::BasicNode * n )
{
	m_nodesStates.Acivate( n );
}

// *******************************
//
bool		Scroller::IsActive			( bv::model::BasicNode * n )
{
	return m_nodesStates.IsActive( n );
}

// ***********************
//
bool        Scroller::CheckLowBuffer      ()
{
    Int32 numActive = (Int32)m_nodesStates.m_actives.size();
    if( numActive > 0 )
    {
        auto lastShift = m_shifts[ m_nodesStates.m_actives[ numActive - 1 ] ];

        Float32 rectCenter;
        Float32 rectDim;

        if( m_scrollDirection == Scroller::ScrollDirection::SD_Down || m_scrollDirection == Scroller::ScrollDirection::SD_Up )
        {
            rectCenter = ( m_view->ymin + m_view->ymax ) / 2;
            rectDim = m_view->Height();
        }
        else // ( m_scrollDirection == Scroller::ScrollDirection::SD_Right || m_scrollDirection == Scroller::ScrollDirection::SD_Left )
        {
            rectCenter = ( m_view->xmin + m_view->xmax ) / 2;
            rectDim = m_view->Width();
        }

        bool shouldNotify = abs( rectCenter - lastShift ) < m_lowBufferMultiplier * rectDim;

        if( shouldNotify && !m_lowBufferNotified )
        {
            m_lowBufferNotified = true;
            return true;
        }
    }
    return false;
}

// ***********************
// Avtivate node and send to the end of nodes queue.
void        Scroller::ShiftNodeToEnd      ( bv::model::BasicNode * n )
{
    m_nodesStates.Acivate( n );

    Int32 lastIdx = (Int32)m_nodesStates.ActiveSize() - 1;
    assert( lastIdx > 0 );

    if( lastIdx > 1 )
    {
        Float32 currShift = m_shifts[ m_nodesStates.m_actives[ lastIdx - 1 ] ];
        currShift += ShiftStep( m_nodesStates.m_actives[ lastIdx - 1 ], m_nodesStates.m_actives[ lastIdx ] );
		m_shifts[ m_nodesStates.m_actives[ lastIdx ] ] = currShift;
    }
    else
        m_shifts[ m_nodesStates.m_actives[ lastIdx ] ] = InitialShift( m_nodesStates.m_actives[ lastIdx ] );
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                Scroller::Serialize       ( ISerializer& ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );
        ser.SetAttribute( "type", m_type );

        if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
        {
            SerializationHelper::SerializeRect( ser, m_view );

            ser.SetAttribute( "Speed", SerializationHelper::T2String( m_speed ) );
            ser.SetAttribute( "Spacing", SerializationHelper::T2String( m_interspace ) );
            ser.SetAttribute( "ScrollDirection", SerializationHelper::T2String( m_scrollDirection ) );
            ser.SetAttribute( "EnableEvents", SerializationHelper::T2String( m_enableEvents ) );
            ser.SetAttribute( "OffscreenNodeBehavior", SerializationHelper::T2String( m_offscreenNodeBehavior ) );
            ser.SetAttribute( "SmoothTime", SerializationHelper::T2String( GetSmoothTime() ) );


            // Node names aren't enough to identify node. Checking children indicies.
            SizeType numChildren = m_parentNode->GetNumChildren();
            std::vector<bv::model::BasicNode*>     childrenNodes;
            childrenNodes.reserve( numChildren );
        
            // Copy all node's to vector
            for( Int32 i = 0; i < numChildren; ++i )
                childrenNodes.push_back( m_parentNode->GetChild( i ).get() );


            ser.EnterArray( "scrollerNodes" );
                for( auto& node : m_shifts )
                {
                    ser.EnterChild( "scrollerNode" );
                        ser.SetAttribute( "name", node.first->GetName() );

                        // Find node index
                        Int32  nodeIndex = -1;
                        for( Int32 i = 0; i < numChildren; ++i )
                        {
                            if( childrenNodes[ i ] == node.first )
                            {
                                nodeIndex = i;
                                break;
                            }
                        }

                        assert( nodeIndex >= 0 );   // Node held by Scroller exists in tree no more.
                        ser.SetAttribute( "nodeIdx", SerializationHelper::T2String( nodeIndex ) );

                    ser.ExitChild(); // node
                }

            ser.ExitChild(); // nodes
        }

    ser.ExitChild(); // logic
}

// ***********************
//
ScrollerPtr      Scroller::Create          ( const IDeserializer & deser, bv::model::BasicNodePtr parent )
{
    mathematics::RectPtr rect = SerializationHelper::CreateRect( deser );

    float speed = SerializationHelper::String2T( deser.GetAttribute( "Speed" ), 0.0f );
    float interspace = SerializationHelper::String2T( deser.GetAttribute( "Spacing" ), 0.0f );
    ScrollDirection scrollDirection = SerializationHelper::String2T( deser.GetAttribute( "ScrollDirection" ), ScrollDirection::SD_Left );
    bool enableEvents = SerializationHelper::String2T( deser.GetAttribute( "EnableEvents" ), false );
    auto offscreenBahavior = SerializationHelper::String2T( deser.GetAttribute( "OffscreenNodeBehavior" ), OffscreenNodeBehavior::ONB_Looping );
    float smoothTime = SerializationHelper::String2T( deser.GetAttribute( "SmoothTime" ), 1.0f );

    auto scroller = Scroller::Create( parent, rect );
    scroller->SetSpeed( speed );
    scroller->SetInterspace( interspace );
    scroller->SetScrollDirection( scrollDirection );
    scroller->SetEnableEvents( enableEvents );
    scroller->SetOffscreenNodeBehavior( offscreenBahavior );
    scroller->SetSmoothTime( smoothTime );


    if( !deser.EnterChild( "scrollerNodes" ) )
        return scroller;
    
    if( deser.EnterChild( "scrollerNode" ) )
    {
        do
        {
            UInt32 nodeIdx = SerializationHelper::String2T( deser.GetAttribute( "nodeIdx" ), -1 );
            
            assert( nodeIdx >= 0 && nodeIdx < parent->GetNumChildren() );
            if( nodeIdx >= 0 && nodeIdx < parent->GetNumChildren() )
            {
                scroller->AddNext( nodeIdx );
            }

        } while( deser.NextChild() );
        deser.ExitChild(); // scrollerNode
    }

    deser.ExitChild(); // scrollerNodes

    return scroller;
}

// ========================================================================= //
// Communication with outer world
// ========================================================================= //

// ***********************
//
bool                Scroller::HandleEvent     ( IDeserializer& eventDeser, ISerializer& response, BVProjectEditor * editor )
{
    std::string scrollAction = eventDeser.GetAttribute( "Action" );

    if( m_scrollerNodePath == "" )
    {
        // Fixme: This should be done in create function, but deserialization from XML doesn't provide
        // node path. Instead we save this path when Scroller first time starts.
        auto context = static_cast<BVDeserializeContext*>( eventDeser.GetDeserializeContext() );
        assert( context != nullptr );

        SetNodePath( context->GetNodePath() );
        m_sceneName = context->GetSceneName();
        m_editor = editor;
    }


	if( scrollAction == "Stop" )
	{
		return Stop();
	}
	else if( scrollAction == "Start" )
	{
		return Start();
	}
    else if( scrollAction == "Reset" )
	{
		return Reset();
	}
    else if( scrollAction == "Pause" )
    {
        return Pause();
    }
    else if( scrollAction == "Clear" )
    {
        return Clear();
    }
    else if( scrollAction == "SmoothStart" )
    {
        return SmoothStart();
    }
    else if( scrollAction == "SmoothPause" )
    {
        return SmoothPause();
    }
    else if( scrollAction == "AddNode" )
    {
        std::string newNode = eventDeser.GetAttribute( "NodeName" );
        AddNext( newNode );
    }
    else if( scrollAction == "AddPreset" )
    {
        return AddPreset( eventDeser, response, editor );
    }
    else if( scrollAction == "AddPresetAndFillWithData" )
    {
        return AddPresetAndMessages( eventDeser, response, editor );
    }
    else if( scrollAction == "SetSpeed" )
	{
        std::string param = eventDeser.GetAttribute( "Speed" );
        float speed = SerializationHelper::String2T( param, 0.0f );

		SetSpeed( speed );
	}
    else if( scrollAction == "GetSpeed" )
    {
        response.SetAttribute( "Speed", SerializationHelper::T2String( m_speed ) );
    }
    else if( scrollAction == "GetStatus" )
    {
        return GetStatus( eventDeser, response, editor );
    }
    else if( scrollAction == "SetScrollDirection" )
    {
        m_scrollDirection = SerializationHelper::String2T( eventDeser.GetAttribute( "ScrollDirection" ), ScrollDirection::SD_Left );
    }
    else if( scrollAction == "GetScrollDirection" )
    {
        response.SetAttribute( "ScrollDirection", SerializationHelper::T2String( m_scrollDirection ) );
    }
    else if( scrollAction == "GetEnableEvents" )
    {
        response.SetAttribute( "EnableEvents", SerializationHelper::T2String( m_enableEvents ) );
    }
    else if( scrollAction == "SetEnableEvents" )
    {
        m_enableEvents = SerializationHelper::String2T( eventDeser.GetAttribute( "ScrollDirection" ), false );
    }
    else if( scrollAction == "SetSpacing" )
    {
        m_interspace = SerializationHelper::String2T( eventDeser.GetAttribute( "Spacing" ), 0.0f );
    }
    else if( scrollAction == "GetSpacing" )
    {
        response.SetAttribute( "Spacing", SerializationHelper::T2String( m_interspace ) );
    }
    else if( scrollAction == "GetOffscreenNodeBehavior" )
    {
        response.SetAttribute( "OffscreenNodeBehavior", SerializationHelper::T2String( m_offscreenNodeBehavior ) );
    }
    else if( scrollAction == "SetOffscreenNodeBehavior" )
    {
        m_offscreenNodeBehavior = SerializationHelper::String2T( eventDeser.GetAttribute( "OffscreenNodeBehavior" ), OffscreenNodeBehavior::ONB_SetNonActive );
    }
    else if( scrollAction == "SetSmoothTime" )
    {
        auto smoothTime = SerializationHelper::String2T( eventDeser.GetAttribute( "SmoothTime" ), 1.0f );
        SetSmoothTime( smoothTime );
    }
    else if( scrollAction == "GetSmoothTime" )
    {
        response.SetAttribute( "SmoothTime", SerializationHelper::T2String( GetSmoothTime() ) );
    }

    return true;
}

// *******************************
//
bool		Scroller::Start			()
{
	if( !m_started )
	{
        Finalize();

		m_started = true;
        m_paused = false;
        m_lowBufferNotified = false;
        m_currTime = Time::Now();
	}
    else if( m_paused )
    {
        m_paused = false;
    }
    else
        return false;

    return true;
}

// *******************************
//
bool		Scroller::Stop			()
{
    return Reset();
}

// ***********************
//
bool        Scroller::Pause               ()
{
    m_paused = true;
    return true;
}

// ***********************
//
bool       Scroller::Clear               ()
{
    auto scene = m_editor->GetScene( m_sceneName );

    for( auto node : m_nodesStates.m_actives )
    {
        m_editor->DeleteChildNode( scene, m_parentNode->shared_from_this(), node->shared_from_this() );
    }

    for( auto node : m_nodesStates.m_nonActives )
    {
        m_editor->DeleteChildNode( scene, m_parentNode->shared_from_this(), node->shared_from_this() );
    }

    m_nodesStates.Clear();
    m_shifts.clear();

    m_paused = false;
    m_started = false;
    m_isFinalized = false;
    m_smoothStart = false;
    m_smoothPause = false;

    return true;
}

// *******************************
//
bool		Scroller::Reset()
{
    // Temporary copy node's to active node's and than set proper layout.
    // Layout is set only for active node's, thats why we have to avtivate them
    auto copy = m_nodesStates.m_nonActives;
    for( auto n : copy )	
        m_nodesStates.Acivate( n );

    LayoutNodes();
    Unfinalize();       // Unfinalize will deactivate all node's.

    m_started = false;
    m_paused = false;
    m_smoothPause = false;
    m_smoothStart = false;

    return true;
}

// ***********************
//
bool        Scroller::SmoothStart         ()
{
    // Prevent from making new smooth start/pause, before ending previous.
    if( m_smoothStart || m_smoothPause )
        return false;

    m_smoothStart = true;
    m_smoothStartTime = Time::Now();

    return Start();
}

// ***********************
//
bool        Scroller::SmoothPause         ()
{
    // Prevent from making new smooth start/pause, before ending previous.
    if( m_smoothStart || m_smoothPause || m_paused )
        return false;

    m_smoothPause = true;
    m_smoothStartTime = Time::Now() - m_smoothTime;

    return true;
}

// ***********************
//
bool        Scroller::GetStatus           ( IDeserializer & /*eventSer*/, ISerializer & response, BVProjectEditor * /*editor*/ )
{
    if( m_started && !m_paused )
    {
        response.SetAttribute( "Status", "Running" );
    }
    else if( m_started && m_paused )
    {
        response.SetAttribute( "Status", "Paused" );
    }
    else
    {
        response.SetAttribute( "Status", "Stopped" );
    }
    return true;
}

// *******************************
//
void		Scroller::SetInterspace		( Float32 interspace )
{
	m_interspace = interspace;
}

// ***********************
//
void        Scroller::SetNodePath         ( std::string nodePath )
{
    m_scrollerNodePath = nodePath;
}

// ***********************
//
void        Scroller::SetScrollDirection  ( ScrollDirection scrollDirection )
{
    m_scrollDirection = scrollDirection;
}

// ***********************
//
void        Scroller::SetEnableEvents     ( bool enable )
{
    m_enableEvents = enable;
}

// ***********************
//
bool        Scroller::SetSmoothTime       ( Float32 time )
{
    m_smoothTime = static_cast< UInt64 >( time * 1000 );
    if( m_smoothTime <= 0 )
    {
        m_smoothTime = 1;
        return false;
    }
    return true;
}

Float32     Scroller::GetSmoothTime       () const
{
    return static_cast< Float32 >( m_smoothTime / 1000 ) ;
}

// ***********************
//
void        Scroller::SetLowBufferMult  ( Float32 lowBufferMult )
{
    m_lowBufferMultiplier = lowBufferMult;
}

// *******************************
//
void		Scroller::SetSpeed			( Float32 speed )
{
    if( speed < 0.0f )
        speed = 0.0f;

	m_speed = speed;
}

// ***********************
//
void        Scroller::SetOffscreenNodeBehavior    ( OffscreenNodeBehavior behavior )
{
    m_offscreenNodeBehavior = behavior;
}

// ***********************
//
bool            Scroller::AddPreset           ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    auto node = CreatePreset( eventSer, response, editor );
    if( node == nullptr )
        return false;

    return AddPresetToScene( eventSer, response, editor, node );
}

// ***********************
//
bool            Scroller::AddPresetAndMessages( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    auto addedNode = CreatePreset( eventSer, response, editor );
    if( addedNode == nullptr )
        return false;

    AddTexts( eventSer, response, editor, addedNode );
    AddImages( eventSer, response, editor, addedNode );

    return AddPresetToScene( eventSer, response, editor, addedNode );
}

// ***********************
//
model::BasicNodePtr Scroller::CreatePreset    ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    auto context = static_cast<BVDeserializeContext*>( eventSer.GetDeserializeContext() );

    std::string newNodeName = eventSer.GetAttribute( "NewNodeName" );
    std::string projectName = eventSer.GetAttribute( "PresetProjectName" );
    std::string presetPath = eventSer.GetAttribute( "PresetPath" );

        
    //auto timeline = editor->GetTimeEvaluator( timelinePath );
    auto timeline = editor->GetTimeEvaluator( context->GetSceneName() );
    auto scene = editor->GetScene( context->GetSceneName() );

    if( timeline == nullptr )
    {
        response.SetAttribute( "ErrorInfo", "Timeline not found" );
        return nullptr;
    }

    auto node = ProjectManager::GetInstance()->LoadPreset( projectName, presetPath, std::static_pointer_cast<bv::model::OffsetTimeEvaluator>( timeline ) );
    if( node == nullptr )
    {
        response.SetAttribute( "ErrorInfo", "Preset not found" );
        return nullptr;
    }

    node->SetName( newNodeName );
    return node;
}

// ***********************
//
bool            Scroller::AddPresetToScene( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor, model::BasicNodePtr node )
{
    auto context = static_cast<BVDeserializeContext*>( eventSer.GetDeserializeContext() );
    auto scene = editor->GetScene( context->GetSceneName() );

    if( scene == nullptr )
    {
        response.SetAttribute( "ErrorInfo", "Scene not found" );
        return nullptr;
    }

    if( !editor->AddChildNode( scene, m_parentNode->shared_from_this(), node ) )
        return false;

    if( AddNode( node ) )
    {
        // Prepare response. Send path to new node.
        std::string addedNodePath = context->GetNodePath() + "/#" + SerializationHelper::T2String( m_parentNode->GetNumChildren() - 1 );
        response.SetAttribute( "AddedNodePath", addedNodePath );
        return true;
    }
    return false;
}

// ***********************
//
model::IParameterPtr                     Scroller::GetParameter        ( const std::string & ) const
{
    return nullptr;
}

// ***********************
//
const std::vector< model::IParameterPtr > & Scroller::GetParameters    () const
{
    static std::vector< model::IParameterPtr > ret;

    return ret;
}

// ***********************
//
void            Scroller::AddTexts            ( IDeserializer & eventSer, ISerializer & /*response*/, BVProjectEditor * editor, model::BasicNodePtr node )
{
    UInt32 textsCounter = 1;
    if( eventSer.EnterChild( "TextsArray" ) )
    {
        if( eventSer.EnterChild( "Text" ) )
        {
            do
            {
                std::string setText = eventSer.GetAttribute( "Text" );
                std::string searchedNode = "text_" + SerializationHelper::T2String( textsCounter );

                auto foundNode = editor->FindNode( node, searchedNode );
                if( foundNode != nullptr )
                {
                    auto textPlugin = foundNode->GetPlugin( "text" );
                    if( textPlugin != nullptr )
                    {
                        auto textParam = textPlugin->GetParameter( "text" );
                        assert( textParam != nullptr );

                        model::SetParameter( textParam, (TimeType)0.0f, StringToWString( setText ).ham );
                    }
                }

                textsCounter++;
            } while( eventSer.NextChild() );
            eventSer.ExitChild();  // Text
        }
        eventSer.ExitChild();   // TextsArray
    }
}

// ***********************
//
void            Scroller::AddImages           ( IDeserializer & eventSer, ISerializer & /*response*/, BVProjectEditor * editor, model::BasicNodePtr node )
{
    UInt32 imgCounter = 1;
    if( eventSer.EnterChild( "ImagesArray" ) )
    {
        if( eventSer.EnterChild( "Image" ) )
        {
            do
            {
                std::string texPath = eventSer.GetAttribute( "TexturePath" );
                std::string searchedNode = "image_" + SerializationHelper::T2String( imgCounter );

                auto foundNode = editor->FindNode( node, searchedNode );
                if( foundNode != nullptr )
                {
                    auto texturePlugin = foundNode->GetPlugin( "texture" );
                    if( texturePlugin != nullptr )
                    {
                        auto textDesc = ProjectManager::GetInstance()->GetAssetDesc( "", "textures", texPath );
                        editor->LoadAsset( texturePlugin, textDesc );
                    }
                }

                imgCounter++;
            } while( eventSer.NextChild() );
            eventSer.ExitChild();  // Text
        }
        eventSer.ExitChild();   // TextsArray
    }
}



} // nodelogic
} // bv