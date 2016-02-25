#include "Crawler.h"

#include "Engine/Models/BasicNode.h"

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"

#include "Engine/Models/Plugins/Simple/DefaultTextPlugin.h"
#include "Engine/Events/Interfaces/IEventManager.h"

#include "CrawlerEvents.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"
#include "Serialization/BV/BVDeserializeContext.h"
#include "Serialization/BV/BVSerializeContext.h"
#include "Tools/StringHeplers.h"

#include "Mathematics/glm_inc.h"

#include "ProjectManager.h"
#include "Engine/Models/BVProjectEditor.h"

#include <algorithm>
#include <ctime>

namespace bv {
    
namespace SerializationHelper {


std::pair< bv::nodelogic::Crawler::CrawlDirection, const char* > CrawlDirectionMapping[] = 
    { std::make_pair( bv::nodelogic::Crawler::CrawlDirection::CD_Down, "CrawlDown" )
    , std::make_pair( bv::nodelogic::Crawler::CrawlDirection::CD_Up, "CrawlUp" )
    , std::make_pair( bv::nodelogic::Crawler::CrawlDirection::CD_Left, "CrawlLeft" )
    , std::make_pair( bv::nodelogic::Crawler::CrawlDirection::CD_Right, "CrawlRight" )
    , std::make_pair( bv::nodelogic::Crawler::CrawlDirection::CD_Total, "" )      // default
};

template<> bv::nodelogic::Crawler::CrawlDirection   String2T        ( const std::string & s, const bv::nodelogic::Crawler::CrawlDirection & defaultVal )    { return String2Enum( CrawlDirectionMapping, s, defaultVal ); }
template<> std::string                              T2String        ( const bv::nodelogic::Crawler::CrawlDirection & t )                                    { return Enum2String( CrawlDirectionMapping, t ); }
    
}   // SerializationHelper
    
namespace nodelogic {


const std::string   Crawler::m_type = "crawler";

// ***********************
//
const std::string   Crawler::GetType             () const
{
    return m_type;
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

glm::vec3       CrawlerShiftToVec   ( Crawler::CrawlDirection crawlDirection )
{
    glm::vec3 shiftDirection;
    if( crawlDirection == Crawler::CrawlDirection::CD_Down )
        shiftDirection = glm::vec3( 0.0, -1.0, 0.0 );
    else if( crawlDirection == Crawler::CrawlDirection::CD_Up )
        shiftDirection = glm::vec3( 0.0, 1.0, 0.0 );
    else if( crawlDirection == Crawler::CrawlDirection::CD_Right )
        shiftDirection = glm::vec3( 1.0, 0.0, 0.0 );
    else if( crawlDirection == Crawler::CrawlDirection::CD_Left )
        shiftDirection = glm::vec3( -1.0, 0.0, 0.0 );
    else
        shiftDirection = glm::vec3( 0.0, 0.0, 0.0 );

    return shiftDirection;
}

} // anonymous


// *******************************
//
CrawlerPtr	Crawler::Create				( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view )
{
	return std::make_shared< Crawler >( parent, view );
}

// *******************************
//
Crawler::Crawler						( bv::model::BasicNode * parent, const mathematics::RectConstPtr & view )
	: m_parentNode( parent )
	, m_isFinalized( false )
	, m_view( view )
	, m_started( false )
	, m_currTime( 0 )
	, m_speed( 0.f )
	, m_interspace( 0.0f )
    , m_paused( false )
    , m_crawlDirection( CrawlDirection::CD_Left )
{}


// ========================================================================= //
// Node managing
// ========================================================================= //


// *******************************
//
void		Crawler::AddNext			( bv::model::BasicNodePtr node )
{
	if(! m_isFinalized )
	{
		m_parentNode->AddChildToModelOnly( node );
		m_nodesStates.Add( node.get() );
	}
	else
		assert(!"Crawler: Cannot add node after finalization!");
}

// ***********************
//
bool		Crawler::AddNext            ( Int32 nodeIdx )
{
    auto newNode = GetNode( m_parentNode, nodeIdx );
    return AddNode( newNode );
}

// ***********************
//
bool		Crawler::AddNext				( const std::string& childNodeName )
{
    auto newNode = GetNode( m_parentNode, childNodeName );
    return AddNode( newNode );
}

// ***********************
//
bool        Crawler::AddNode             ( bv::model::BasicNodePtr node )
{
    if(! m_isFinalized )
	{
        bool alreadyExists = m_nodesStates.Exist( node.get() );
        assert( !alreadyExists );

        if( !alreadyExists )
        {
		    m_nodesStates.Add( node.get() );
            return true;
        }
	}
	else
		assert(!"Crawler: Cannot add node after finalization!");

    return false;
}


// *******************************
//
bool		Crawler::Finalize			()
{
	if( m_isFinalized )
		;//assert(!"Crawler: Already finalized!");
	else
	{
        glm::vec3 shiftDirection = CrawlerShiftToVec( m_crawlDirection );

		auto copy = m_nodesStates.m_nonActives;
		for( auto n : copy )
		{
			m_shifts[ n ] = m_view->xmax;
			UpdateVisibility( n );
			auto trPlugin = n->GetPlugin( "transform" );
			if( trPlugin )
			{
				auto trParam = trPlugin->GetParameter( "simple_transform" );
                assert( trParam != nullptr );

				model::SetParameterTranslation( trParam, 0.0f, glm::vec3( 5.0f, 0.0f, 0.0f ) );
			}
		}
		//m_shifts[ 0 ]+=m_interspace;
		for( auto elem : m_shifts )
		{
			//if( IsActive( elem.first ) )
			{
				auto trPlugin = elem.first->GetPlugin( "transform" );
				if( trPlugin )
				{
					auto trParam = trPlugin->GetParameter( "simple_transform" );
					model::SetParameterTranslation( trParam, 0.0f, shiftDirection * -elem.second );
				}
			}
		}
		for( auto n : copy )	
			SetActiveNode( n );

		LayoutNodes();
		m_isFinalized = true;
	}

	return m_isFinalized;
}

// ***********************
//
bool        Crawler::Unfinalize          ()
{
    auto copy = m_nodesStates.m_nonActives;

    for( auto n : copy )	
		m_nodesStates.Deacivate( n );
    
    m_started = false;
    m_isFinalized = false;

    return true;
}


// *******************************
//
void		Crawler::LayoutNodes		()
{
	auto length = m_nodesStates.ActiveSize();
	if( length > 0 )
	{
		Float32 currShift = m_view->xmax + m_interspace;

		m_shifts[ m_nodesStates.m_actives[ 0 ] ] = currShift;

		for( SizeType i = 1; i < length; ++i )
		{
			currShift += m_nodesStates.m_actives[ i - 1 ]->GetAABB().Width() + m_interspace;

			m_shifts[ m_nodesStates.m_actives[ i ] ] = currShift;
		}

		UpdateTransforms();
	}
}

// *******************************
//
void		Crawler::AddMessage			(std::wstring msg)
{
	m_messages_new.push_back(msg);
	
	
}
// *******************************
//
void		Crawler::Clear()
{
	m_messages_new.clear();
	m_messages_displayed.clear();

}


// *******************************
//
void		Crawler::Reset()
{
	m_nodesStates.m_visibles.clear();
	/*while(m_nodesStates.m_actives.size()>0)
	{
		BasicNode *node = m_nodesStates.m_actives.push_back();


	}*/

	m_nodesStates.m_nonActives.insert(m_nodesStates.m_nonActives.end(), m_nodesStates.m_actives.begin(), m_nodesStates.m_actives.end());

	m_nodesStates.m_actives.clear();
	m_nodesStates.m_visibles.clear();


	for( auto elem : m_shifts )
	{
		
			auto trPlugin = elem.first->GetPlugin( "transform" );
			if( trPlugin )
			{
				auto trParam = trPlugin->GetParameter( "simple_transform" );
				model::SetParameterTranslation( trParam, 0.0f, glm::vec3( 5.0f, 0.0f, 0.0f ) );
			}
			UpdateVisibility(elem.first );
		
	}

	
		
	Finalize();



}

// *******************************
//
void		Crawler::SetPromoMessage			(std::wstring msg)
{
	m_promo_msg = msg;
}

// *******************************
//
void		Crawler::SetPromoFrequency			(int freq)
{
	m_promo_freq = freq;
}


// *******************************
//
void		Crawler::Update				( TimeType )
{
    if( m_started && !m_paused )
	{
        auto t = std::clock();
		auto shift = m_speed * ( ( t - m_currTime ) / 1000.f );

		m_currTime = t;

		if( shift > 0.f )
		{
			for( auto elem : m_shifts )
				m_shifts[ elem.first ] -= shift;

			UpdateTransforms();
		}
	}
}

// *******************************
//
void		Crawler::UpdateTransforms	()
{
    glm::vec3 shiftDirection = CrawlerShiftToVec( m_crawlDirection );

	for( auto elem : m_shifts )
	{
		if( IsActive( elem.first ) )
		{
			auto trPlugin = elem.first->GetPlugin( "transform" );
			if( trPlugin )
			{
				auto trParam = trPlugin->GetParameter( "simple_transform" );
				model::SetParameterTranslation( trParam, 0.0f, shiftDirection * -elem.second );
			}
		}
	}

	auto copy = m_nodesStates.m_actives;

	for( auto n : copy )
		UpdateVisibility( n );

	if( m_nodesStates.NonActiveSize() > 0 && m_nodesStates.ActiveSize() == m_nodesStates.VisibleSize() )
		NotifyNoMoreNodes();
}

// *******************************
//
void		Crawler::UpdateVisibility	( bv::model::BasicNode * n )
{
	auto currVisibility = m_nodesStates.IsVisible( n );
	auto nAABB = n->GetAABB();
	bool newVisibility = nAABB.HasNonEmptyIntersection( *m_view );

	if( currVisibility != newVisibility )
	{
		if( newVisibility )
		{
			m_nodesStates.Visible( n );
			NotifyVisibilityChanged( n, newVisibility );
		}
		else if( IsActive( n ) )
		{
			m_nodesStates.NotVisible( n );
			NotifyVisibilityChanged( n, newVisibility );
		}
	}
}

// *******************************
//
void		Crawler::NotifyVisibilityChanged( bv::model::BasicNode * n, bool visibility )
{
	auto & eventManager = GetDefaultEventManager();

	if( visibility )
		eventManager.TriggerEvent( std::make_shared< NodeAppearingCrawlerEvent >( shared_from_this(), n ) );
	else
		eventManager.TriggerEvent( std::make_shared< NodeLeavingCrawlerEvent >( shared_from_this(), n ) );

	//printf( "Visibility of %p changed on %i \n", n, visibility );
	//printf( "Active : %i NonActive: %i Visible %i \n", m_nodesStates.ActiveSize(), m_nodesStates.NonActiveSize(), m_nodesStates.VisibleSize() );
}

// *******************************
//
void		Crawler::NotifyNoMoreNodes( )
{
	auto & eventManager = GetDefaultEventManager();
	eventManager.TriggerEvent( std::make_shared< NoMoreNodesCrawlerEvent >( shared_from_this() ) );
	HackNoMoreNodes();
}

// *******************************
//
void		Crawler::HackNoMoreNodes( )
{
	auto n = this->GetNonActiveNode();
	std::wstring	 message	=	L"";

	if(m_total_displayed_msgs % m_promo_freq == 0)
	{
		message = m_promo_msg;
	}else if(m_messages_new.size()>0)
	{
		message = m_messages_new[0];
		m_messages_displayed.push_back(message);
		m_messages_new.erase(m_messages_new.begin());
	}else
		if(m_messages_displayed.size()>0)
		{
			if((SizeType)m_displayed_index >= m_messages_displayed.size())
			{
				m_displayed_index = 0;
			}

			message = m_messages_displayed[m_displayed_index];
			m_displayed_index++;
		
	}else{
		message = m_promo_msg;
	}

	if( n && message!=L"")
	{
		auto textNode = n->GetChild( "Text" );
		if( textNode )
		{
			auto pl = textNode->GetPlugin( "text" );

			if( pl )
			{
                SetParameter( pl->GetParameter( "text" ), 0.0, message );
                this->EnqueueNode( n );
			}
		}
	}
	m_total_displayed_msgs++;


	
}

// *******************************
//
void		Crawler::SetActiveNode		( bv::model::BasicNode * n )
{
	m_nodesStates.Acivate( n );
}

// *******************************
//
bool		Crawler::IsActive			( bv::model::BasicNode * n )
{
	return m_nodesStates.IsActive( n );
}

// *******************************
//
model::BasicNode *	Crawler::GetNonActiveNode()
{
	if ( m_nodesStates.NonActiveSize() > 0 )
		return m_nodesStates.m_nonActives[ 0 ];
	else
		return nullptr;
}

// *******************************
//
void		Crawler::EnqueueNode			( model::BasicNode * n)
{
	if( m_nodesStates.IsNonActive( n ) )
	{
		auto activeSize = m_nodesStates.ActiveSize();
		if( activeSize > 0 )
		{
			auto lastActiveNode = m_nodesStates.m_actives[ activeSize - 1 ];

			auto nodeShift = m_shifts[ lastActiveNode ] + lastActiveNode->GetAABB().Width() + m_interspace;

			//nodeShift = max( nodeShift, m_view->xmax );

			m_shifts[ n ] = nodeShift;
			m_nodesStates.Acivate( n );
			UpdateTransforms();
		}
		else
		{
			auto nodeShift = m_view->xmax+m_interspace;

			m_shifts[ n ] = nodeShift;
			m_nodesStates.Acivate( n );
			UpdateTransforms();
		}
	}
}

// ========================================================================= //
// Serialization and deserialization
// ========================================================================= //

// ***********************
//
void                Crawler::Serialize       ( ISerializer& ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );
    assert( context != nullptr );

    ser.EnterChild( "logic" );
        ser.SetAttribute( "type", m_type );

        if( context->detailedInfo )     // Without detailed info, we need to serialize only logic type.
        {
            ser.EnterChild( "view" );
                ser.SetAttribute( "empty", SerializationHelper::T2String( m_view->m_empty ) );
                if( !m_view->m_empty )
                {
                    ser.SetAttribute( "xmin", SerializationHelper::T2String( m_view->xmin ) );
                    ser.SetAttribute( "xmax", SerializationHelper::T2String( m_view->xmax ) );
                    ser.SetAttribute( "ymin", SerializationHelper::T2String( m_view->ymin ) );
                    ser.SetAttribute( "ymax", SerializationHelper::T2String( m_view->ymax ) );
                }
            ser.ExitChild(); // view

            ser.SetAttribute( "speed", SerializationHelper::T2String( m_speed ) );
            ser.SetAttribute( "interspace", SerializationHelper::T2String( m_interspace ) );


            // Node names aren't enough to identify node. Checking children indicies.
            SizeType numChildren = m_parentNode->GetNumChildren();
            std::vector<bv::model::BasicNode*>     childrenNodes;
            childrenNodes.reserve( numChildren );
        
            // Copy all node's to vector
            for( Int32 i = 0; i < numChildren; ++i )
                childrenNodes.push_back( m_parentNode->GetChild( i ).get() );


            ser.EnterArray( "crawlerNodes" );
                for( auto& node : m_shifts )
                {
                    ser.EnterChild( "crawlerNode" );
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

                        assert( nodeIndex >= 0 );   // Node held by crawler exists in tree no more.
                        ser.SetAttribute( "nodeIdx", SerializationHelper::T2String( nodeIndex ) );

                    ser.ExitChild(); // node
                }

            ser.ExitChild(); // nodes
        }

    ser.ExitChild(); // logic
}

// ***********************
//
CrawlerPtr      Crawler::Create          ( const IDeserializer & deser, bv::model::BasicNode * parent )
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

    float speed = SerializationHelper::String2T( deser.GetAttribute( "speed" ), 0.0f );
    float interspace = SerializationHelper::String2T( deser.GetAttribute( "interspace" ), 0.0f );

    auto crawler = Crawler::Create( parent, rect );
    crawler->SetSpeed( speed );
    crawler->SetInterspace( interspace );


    if( !deser.EnterChild( "crawlerNodes" ) )
        return crawler;
    
    if( deser.EnterChild( "crawlerNode" ) )
    {
        do
        {
            UInt32 nodeIdx = SerializationHelper::String2T( deser.GetAttribute( "nodeIdx" ), -1 );
            
            assert( nodeIdx >= 0 && nodeIdx < parent->GetNumChildren() );
            if( nodeIdx >= 0 && nodeIdx < parent->GetNumChildren() )
            {
                crawler->AddNext( nodeIdx );
            }

        } while( deser.NextChild() );
        deser.ExitChild(); // crawlerNode
    }

    deser.ExitChild(); // crawlerNodes

    return crawler;
}

// ========================================================================= //
// Communication with outer world
// ========================================================================= //

// ***********************
//
bool                Crawler::HandleEvent     ( IDeserializer& eventSer, ISerializer& response, BVProjectEditor * editor )
{
    std::string crawlAction = eventSer.GetAttribute( "Action" );

	if( crawlAction == "Stop" )
	{
		Stop();
	}
	else if( crawlAction == "Start" )
	{
		Start();
	}
    else if( crawlAction == "Reset" )
	{
		Reset();
	}
    else if( crawlAction == "Pause" )
    {
        Pause();
    }
    else if( crawlAction == "AddNode" )
    {
        std::string newNode = eventSer.GetAttribute( "NodeName" );
        AddNext( newNode );
    }
    else if( crawlAction == "AddPreset" )
    {
        return AddPreset( eventSer, response, editor );
    }
    else if( crawlAction == "AddPresetAndFillWithData" )
    {
        return AddPresetAndMessages( eventSer, response, editor );
    }
    else if( crawlAction == "SetSpeed" )
	{
        std::string param = eventSer.GetAttribute( "Speed" );
        float speed = SerializationHelper::String2T( param, 0.5f );

		SetSpeed( speed );
	}
    else if( crawlAction == "GetSpeed" )
    {
        response.SetAttribute( "Speed", SerializationHelper::T2String( m_speed ) );
    }
    else if( crawlAction == "GetStatus" )
    {
        return GetStatus( eventSer, response, editor );
    }
    else if( crawlAction == "SetCrawlDirection" )
    {
        m_crawlDirection = SerializationHelper::String2T( eventSer.GetAttribute( "CrawlDirection" ), CrawlDirection::CD_Left );
    }
    else if( crawlAction == "GetCrawlDirection" )
    {
        response.SetAttribute( "CrawlDirection", SerializationHelper::T2String( m_crawlDirection ) );
    }
    // Deprecated
    else if( crawlAction == "Finalize" )
    {
        Finalize();
    }
    else if( crawlAction == "Unfinalize" )
    {
        Unfinalize();
    }
    else if( crawlAction == "AddText" )
	{
        std::string param = eventSer.GetAttribute( "Message" );
        AddMessage( StringToWString( param ) );
	}
	else if( crawlAction == "Clear" )
	{
		Clear();
	}

    return true;
}



// *******************************
//
void		Crawler::Start			()
{
	if( !m_started )
	{
        Finalize();

		m_started = true;
        m_paused = false;
        m_currTime = std::clock();
	}
    else
    {
        assert( m_paused );
        m_paused = false;
    }
}

// *******************************
//
void		Crawler::Stop			()
{
	m_started = false;

    Unfinalize();
}

// ***********************
//
void        Crawler::Pause               ()
{
    m_paused = true;
}

// ***********************
//
bool            Crawler::GetStatus           ( IDeserializer & /*eventSer*/, ISerializer & response, BVProjectEditor * /*editor*/ )
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
void		Crawler::SetInterspace		( Float32 interspace )
{
	m_interspace = interspace;
}

// *******************************
//
void		Crawler::SetSpeed			( Float32 speed )
{
    if( speed < 0.0f )
        speed = 0.0f;

	m_speed = speed;
}


// ***********************
//
bool            Crawler::AddPreset           ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
{
    auto node = CreatePreset( eventSer, response, editor );
    if( node == nullptr )
        return false;

    return AddPresetToScene( eventSer, response, editor, node );
}

// ***********************
//
bool            Crawler::AddPresetAndMessages( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
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
model::BasicNodePtr Crawler::CreatePreset    ( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor )
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
bool            Crawler::AddPresetToScene( IDeserializer & eventSer, ISerializer & response, BVProjectEditor * editor, model::BasicNodePtr node )
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
void            Crawler::AddTexts            ( IDeserializer & eventSer, ISerializer & /*response*/, BVProjectEditor * editor, model::BasicNodePtr node )
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
void            Crawler::AddImages           ( IDeserializer & eventSer, ISerializer & /*response*/, BVProjectEditor * editor, model::BasicNodePtr node )
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