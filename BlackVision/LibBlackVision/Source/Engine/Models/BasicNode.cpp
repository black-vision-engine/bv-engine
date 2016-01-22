#include "BasicNode.h"

#include <set>

#include "Engine/Models/Builder/NodeLogicHolder.h"

//FIXME: node na INode
#include "Tools/StringHeplers.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"

#include "Engine/Models/ModelNodeEditor.h"

#include "Engine/Models/Plugins/Plugin.h"


#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVSerializeContext.h"
//#include "Serialization/SerializationObjects.inl"
#include "Serialization/BV/CloneViaSerialization.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Assets/AssetDescsWithUIDs.h"

#include "UseLoggerLibBlackVision.h"

namespace bv { 
    
// serialization stuff
//template std::shared_ptr< model::BasicNode >                                        DeserializeObjectLoadImpl( const IDeserializer& pimpl, std::string name );
    
namespace model {

namespace {

// ********************************
//
IModelNodePtr  FindNode( const TNodeVec & vec, const std::string & name )
{
    for( auto node : vec )
    {
        if( node->GetName() == name )
        {
            return node;
        }
    }

    return nullptr;
}

} //anonymous

// ********************************
//
BasicNode::BasicNode( const std::string & name, ITimeEvaluatorPtr, const PluginsManager * pluginsManager )
    : m_name( name )
    , m_pluginList( std::make_shared< DefaultPluginListFinalized >() )
    , m_pluginsManager( pluginsManager )
    , m_visible( true )
	, m_modelNodeEditor ( new ModelNodeEditor( this ) )
    , m_modelNodeEffect( nullptr )
{
    if( pluginsManager == nullptr )
    {
        m_pluginsManager = &PluginsManager::DefaultInstance();
    }
}

// ********************************
//
BasicNode::~BasicNode()
{
    delete m_modelNodeEditor;
}

// ********************************
//
BasicNodePtr                    BasicNode::Create                   ( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager )
{
    struct make_shared_enabler_BasicNode : public BasicNode
    {
        make_shared_enabler_BasicNode( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager )
            : BasicNode( name, timeEvaluator, pluginsManager )
        {
        }
    };
	return std::make_shared< make_shared_enabler_BasicNode >( name, timeEvaluator, pluginsManager );
}

// ********************************
//
void                            BasicNode::Serialize               ( ISerializer& ser ) const
{
    auto context = static_cast<BVSerializeContext*>( ser.GetSerializeContext() );

    ser.EnterChild( "node" );
    ser.SetAttribute( "name", GetName() );

    if( context->detailedInfo )
        ser.SetAttribute( "visible", m_visible ? "true" : "false" );

    if( context->pluginsInfo )
    {
        ser.EnterArray( "plugins" );
            for( unsigned int  i = 0; i < m_pluginList->NumPlugins(); i++ )
            {
                auto plugin_ = m_pluginList->GetPlugin( i );
                auto plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin_ );
                assert( plugin );
                plugin->Serialize( ser );
            }
        ser.ExitChild(); // plugins
    }

    if( context->detailedInfo && m_modelNodeEffect )
        m_modelNodeEffect->Serialize( ser );

    if( context->detailedInfo && m_nodeLogic )
        m_nodeLogic->Serialize( ser );

    if( context->recursive )
    {
        ser.EnterArray( "nodes" );
            for( auto child : m_children )
                child->Serialize( ser );
        ser.ExitChild();
    }

    ser.ExitChild();
}

// ********************************
//
BasicNode * BasicNode::Create( const IDeserializer& deser )
{
    //assert( deser.GetName() == "node" ); FIXME

    auto name = deser.GetAttribute( "name" );

    auto deserContext = Cast< BVDeserializeContext * >( deser.GetDeserializeContext() );

    if( deserContext == nullptr )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "node " << name << " serilization aborded because of an error";
        assert( !"Wrong DeserializeContext casting." );
        return nullptr;
    }

	//FIXME: nullptr because timeEvaluator is not used in BasicNode
    //auto node = Create( name, nullptr );
    auto node = new BasicNode( name, nullptr );

    node->m_visible = deser.GetAttribute( "visible" ) == "false" ? false : true;

// plugins
    deserContext->ClearRendererContextes();
    auto plugins = SerializationHelper::DeserializeArray< BasePlugin< IPlugin > >( deser, "plugins" );

    auto itRC = deserContext->RendererContextes().begin();

    assert( plugins.size() == deserContext->RendererContextes().size() );  // A little bit of defensive programming

    for( auto plugin : plugins )
    {
        node->AddPlugin( plugin );

        // override renderer context
        if( *itRC && plugin->GetPixelShaderChannel() )
        {
            plugin->SetRendererContext( *itRC );
        }

        ++itRC;
    }
    
    auto psc = plugins.back()->GetPixelShaderChannel();

    if( psc )
    {
        HelperPixelShaderChannel::SetRendererContextUpdate( psc );
    }

//@todo Deserialize Global effects; use ModelNodeEffectFactory.

// children
    auto children = SerializationHelper::DeserializeArray< BasicNode >( deser, "nodes" );

    for( auto child : children )
        node->AddChildToModelOnly( child );

// node logic
// Node logic creation should take place always after all children are deserialized.
    if( deser.EnterChild( "logic" ) )
    {
        auto factory = GetNodeLogicFactory();
        auto newLogic = factory->CreateLogic( deser, node );

        if( newLogic )
            node->SetLogic( newLogic );

        deser.ExitChild();  // logic
    }

    return node;
    //SetParamVal("nodePath" ,"plugin", { name: "translataion", type:"vec3" , val:"0 ,0 ,0" } );
}

// *******************************
//
IModelNode *					BasicNode::Clone			() const
{
	auto assets = std::make_shared< AssetDescsWithUIDs >();
	//FIXME: const hack
	GetAssetsWithUIDs( *assets, this );

	return CloneViaSerialization::Clone( this, "node", assets, nullptr );
}

// ********************************
//
IPluginPtr                      BasicNode::GetPlugin                ( const std::string & name ) const
{
    return m_pluginList->GetPlugin( name );
}

// ********************************
//
IFinalizePluginConstPtr BasicNode::GetFinalizePlugin                () const
{
    return m_pluginList->GetFinalizePlugin();
}

// ********************************
//
IModelNodePtr           BasicNode::GetNode                          ( const std::string & path, const std::string & separator )
{
    std::string childPath = path;

    if( childPath.empty() )
	{
		return shared_from_this();
	}

    auto childName = SplitPrefix( childPath, separator );
    auto childIdx = TryParseIndex( childName );
        
    if( childIdx >= 0 )
    {
        if( childIdx < m_children.size() )
        {
            return m_children[ childIdx ]->GetNode( childPath, separator );
        }

        return nullptr;
    }

	for( auto & child : m_children )
	{
        if( child->GetName() == childName )
		{
            return child->GetNode( childPath, separator );
        }
	}

    return nullptr;
}

// ********************************
//
IModelNodePtr                   BasicNode::GetChild                 ( const std::string & name )
{
    return FindNode( m_children, name );
}

// ********************************
//
const IPluginListFinalized *    BasicNode::GetPluginList            () const
{
    return m_pluginList.get();
}

// ********************************
//
std::vector< IParameterPtr >    BasicNode::GetParameters           () const
{
    std::vector< IParameterPtr > ret;

    auto plugins = GetPluginList();

    for( UInt32 i = 0; i < plugins->NumPlugins(); i++ )
    {
        auto params =  plugins->GetPlugin( i )->GetParameters();
        ret.insert( ret.end(), params.begin(), params.end() );

        params = plugins->GetPlugin( i )->GetResourceStateModelParameters();
        ret.insert( ret.end(), params.begin(), params.end() );
    }

	auto effect = GetNodeEffect();
    if( effect )
    {
        auto params =  effect->GetParameters();
        ret.insert( ret.end(), params.begin(), params.end() );
    }

    return ret;
}


// ********************************
//
std::unordered_set< ITimeEvaluatorPtr > BasicNode::GetTimelines			( bool recursive ) const
{
	std::unordered_set< ITimeEvaluatorPtr > timelines;

    auto params = GetParameters();

    for( auto param : params )
    {
        timelines.insert( param->GetTimeEvaluator() );
    }

	if( recursive )
	{
		for( auto child : m_children )
		{
			auto ts = child->GetTimelines( true );
			timelines.insert( ts.begin(), ts.end() ); // FIXME: remove duplicates
		}
	}

    return timelines;
}

// ********************************
//
unsigned int                    BasicNode::GetNumChildren           () const
{
    return (unsigned int) m_children.size();
}

// ********************************
//
IModelNodeEffectPtr             BasicNode::GetNodeEffect            () const
{
    return m_modelNodeEffect;
}

// ********************************
//
void                            BasicNode::SetNodeEffect            ( IModelNodeEffectPtr nodeEffect )
{
    m_modelNodeEffect = nodeEffect;
}

// ********************************
//
const std::string &             BasicNode::GetName                  () const
{
    return m_name;
}

// ********************************
//
void                            BasicNode::SetName                  ( const std::string & name )
{
    m_name = name;
}

// ********************************
//
mathematics::Rect 			    BasicNode::GetAABB			        () const
{
    mathematics::Rect r;

    auto trans = m_pluginList->GetFinalizePlugin()->GetParamTransform()->Evaluate( 0 );

    auto plRect = m_pluginList->GetFinalizePlugin()->GetAABB( trans );

    if( plRect )
        r.Include( *plRect );


    for( auto ch : m_children )
    {
        r.Include( ch->GetAABB( trans ) );
    }

    return r;
}

// ********************************
//
mathematics::Rect 			BasicNode::GetAABB						( const glm::mat4 & parentTransformation ) const
{
    mathematics::Rect r;

    auto trans = parentTransformation * m_pluginList->GetFinalizePlugin()->GetParamTransform()->Evaluate( 0 );

    auto plRect = m_pluginList->GetFinalizePlugin()->GetAABB( trans );

    if( plRect )
        r.Include( *plRect );


    for( auto ch : m_children )
    {
        r.Include( ch->GetAABB( trans ) );
    }

    return r;
}

// ********************************
//
BasicNodePtr    BasicNode::GetChild                         ( unsigned int i )
{
    assert( i < m_children.size() );

    return m_children[ i ];
}

// ********************************
//
const BasicNode *   BasicNode::GetChild                     ( unsigned int i ) const
{
    assert( i < m_children.size() );

    return m_children[ i ].get();
}

// ********************************
//
unsigned int    BasicNode::GetNumPlugins                    () const
{
    return m_pluginList->NumPlugins();
}

// ********************************
//
void            BasicNode::AddChildToModelOnly              ( BasicNodePtr n )
{
	m_children.push_back( n );
}

// ********************************
//
void            BasicNode::AddChildToModelOnly              ( BasicNodePtr n, UInt32 idx )
{
	if( idx < m_children.size() )
	{
		m_children.insert( m_children.begin() + idx, n );
	}
	else
	{
		m_children.push_back( n );
	}
}

// ********************************
//
void            BasicNode::DetachChildNodeOnly              ( BasicNodePtr n )
{
    for( unsigned int i = 0; i < m_children.size(); ++i )
    {
        if( m_children[ i ] == n )
        {
            m_children.erase( m_children.begin() + i );

            return;
        }
    }

    assert( false );
}

// ********************************
//
ModelNodeEditor *					BasicNode::GetModelNodeEditor		()
{
    assert( m_modelNodeEditor );
    return m_modelNodeEditor;
}

// ********************************
//
DefaultPluginListFinalizedPtr		BasicNode::GetPlugins		()
{
    return m_pluginList;
}

// ********************************
//
void            BasicNode::SetPlugins                       ( DefaultPluginListFinalizedPtr plugins )
{
    m_pluginList = plugins;
}

// ********************************
//
bool            BasicNode::AddPlugin                        ( IPluginPtr plugin )
{
    m_pluginList->AttachPlugin( plugin );

    return true;
}

// ********************************
//
bool            BasicNode::AddPlugin                        ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator )
{
    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;
    m_pluginList->AttachPlugin( m_pluginsManager->CreatePlugin( uid, prev, timeEvaluator ) );

    return true;
}

// ********************************
//
bool            BasicNode::AddPlugin                    ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    m_pluginList->AttachPlugin( m_pluginsManager->CreatePlugin( uid, name, prev, timeEvaluator ) );

    return true;
}

// ********************************
//
bool           BasicNode::AddPlugins              ( const std::vector< std::string > & uids, ITimeEvaluatorPtr timeEvaluator )
{
    for( auto uid : uids )
    {
        if( !AddPlugin( uid, timeEvaluator ) )
        {
            return false;
        }
    }

    return true;
}

// ********************************
//
void			BasicNode::SetLogic					( INodeLogicPtr logic )
{
    m_nodeLogic = logic;
    m_nodeLogic->Initialize();
}

// ********************************
//
void BasicNode::Update( TimeType t )
{
    if( IsVisible() )
    {
        if ( m_modelNodeEffect )
        {
            m_modelNodeEffect->Update( t );
        }

        m_pluginList->Update( t );

		if( m_nodeLogic )
        {
		    m_nodeLogic->Update( t );
        }

        for( auto ch : m_children )
        {
            ch->Update( t );
        }
    }
}

// ********************************
//
bool  BasicNode::IsVisible               () const
{
    return m_visible;
}

// ********************************
//
void  BasicNode::SetVisible              ( bool visible )
{
    m_visible = visible;
}

// ********************************
//
INodeLogicPtr                       BasicNode::GetLogic				    () const
{
    return m_nodeLogic;
}

// ********************************
//
std::string                         BasicNode::SplitPrefix              ( std::string & path, const std::string & separator )
{
    assert( separator.length() == 1 );

    path = Trim( path, separator ); //strip unnecessary separators
    auto ret = Split( path, separator );

    if( ret.size() == 0 )
    {
        ret.push_back( "" );
    }
    else if ( ret.size() == 1 )
    {
        path = "";
    }
    else
    {
        path = Join( std::vector< std::string >( ret.begin() + 1, ret.end() ), separator );
    }

    return ret[ 0 ];
}

// ********************************
//
Int32                               BasicNode::TryParseIndex            ( std::string & str, const char escapeChar )
{
    if( !str.empty() && str[ 0 ] == escapeChar )
    {
        Int32 result;
        bool success = ( ( std::stringstream( str.substr( 1, str.length() ) ) >> result ) != nullptr );
        if( success )
        {
            return result;
        }
    }

    return -1;
}

} // model


namespace CloneViaSerialization {

void UpdateTimelines( model::BasicNode * obj, const std::string & prefix, std::string srcScene, std::string destScene, bool recursive )
{
    for( auto param : obj->GetParameters() )
    {
        auto name = model::TimelineManager::GetInstance()->GetTimelinePath( param->GetTimeEvaluator() );
        model::ITimeEvaluatorPtr timeline;
        if( name == srcScene )
            timeline = model::TimelineManager::GetInstance()->GetTimeEvaluator( destScene );
        else
            timeline = model::TimelineManager::GetInstance()->GetTimeEvaluator( model::TimelineHelper::CombineTimelinePath( destScene,  prefix + param->GetTimeEvaluator()->GetName() ) );
        param->SetTimeEvaluator( timeline );
    }

	if( recursive )
		for( unsigned int i = 0; i < obj->GetNumChildren(); i++ )
			UpdateTimelines( obj->GetChild( i ).get(), prefix, srcScene, destScene, true );
}

// *******************************
//FIXME: name of method should indicate that timelines are modified or sth?
model::BasicNodePtr		CloneNode		( const model::BasicNode * obj, const std::string & prefix, std::string srcScene, std::string destScene )
{
    auto clone = static_cast< model::BasicNode* >( obj->Clone() );

	UpdateTimelines( clone, prefix, srcScene, destScene, true );

    return model::BasicNodePtr( dynamic_cast< model::BasicNode* >( clone ) );
}

} //CloneViaSerialization


} // bv
