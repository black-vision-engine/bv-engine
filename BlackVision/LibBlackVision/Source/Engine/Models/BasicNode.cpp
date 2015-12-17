#include "BasicNode.h"

//FIXME: node na INode
#include "Tools/StringHeplers.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"

#include "Engine/Models/ModelNodeEditor.h"

#include "Engine/Models/Plugins/Plugin.h"


#include "Engine/Models/Timeline/TimelineManager.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/BVSerializeContext.h"
//#include "Serialization/SerializationObjects.inl"
#include "Serialization/CloneViaSerialization.h"
#include "Assets/AssetDescsWithUIDs.h"

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
	, m_modelNodeEditor ( nullptr )
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
	auto node = std::make_shared<make_shared_enabler_BasicNode>( name, timeEvaluator, pluginsManager );
	node->m_modelNodeEditor = new ModelNodeEditor( node );
    return node;
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

    ser.EnterArray( "plugins" );
        for( unsigned int  i = 0; i < m_pluginList->NumPlugins(); i++ )
        {
            auto plugin_ = m_pluginList->GetPlugin( i );
            auto plugin = std::static_pointer_cast< BasePlugin< IPlugin > >( plugin_ );
            assert( plugin );
            plugin->Serialize( ser );
        }
    ser.ExitChild(); // plugins

    if( context->detailedInfo && m_modelNodeEffect )
        m_modelNodeEffect->Serialize( ser );

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
ISerializablePtr BasicNode::Create( const IDeserializer& dob )
{
    //assert( dob.GetName() == "node" ); FIXME

    auto name = dob.GetAttribute( "name" );

	//FIXME: nullptr because timeEvaluator is not used in BasicNode
    auto node = Create( name, nullptr );

    node->m_visible = dob.GetAttribute( "visible" ) == "false" ? false : true;

// plugins
    auto plugins = SerializationHelper::DeserializeObjectLoadArrayImpl< BasePlugin< IPlugin > >( dob, "plugins" );
	
    for( auto plugin : plugins )
        node->AddPlugin( plugin );

//// node effect
//    if( m_modelNodeEffect )
//        m_modelNodeEffect->Serialize( dob );

// children
    auto children = SerializationHelper::DeserializeObjectLoadArrayImpl< BasicNode >( dob, "nodes" );

    for( auto child : children )
        node->AddChildToModelOnly( child );

    return node;


    //SetParamVal("nodePath" ,"plugin", { name: "translataion", type:"vec3" , val:"0 ,0 ,0" } );
}

// *******************************
//
IModelNodePtr					BasicNode::Clone			() const
{
	AssetDescsWithUIDs assets;
	//FIXME: const hack
	GetAssetsWithUIDs( assets, std::const_pointer_cast< BasicNode >( shared_from_this() ) );
	AssetDescsWithUIDs::SetInstance( assets );

	return CloneViaSerialization::Clone( this, "node" );
}

// ********************************
//
IPluginPtr                      BasicNode::GetPlugin                ( const std::string & name ) const
{
    return m_pluginList->GetPlugin( name );
}

// ********************************
//
IFinalizePluginConstPtr BasicNode::GetFinalizePlugin        () const
{
    return m_pluginList->GetFinalizePlugin();
}

// ********************************
//
IModelNodePtr           BasicNode::GetNode                  ( const std::string & path, const std::string & separator )
{
    std::string suffix = path;

    auto name = SplitPrefix( suffix, separator );
    if( name == GetName() )
    {
		if( suffix.empty() )
		{
			return shared_from_this();
		}

		for( auto & child : m_children )
		{
			auto node = child->GetNode( suffix, separator );
			if( node )
			{
				return node;
			}
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
std::vector< ITimeEvaluatorPtr > BasicNode::GetTimelines			() const
{
	std::vector< ITimeEvaluatorPtr > ret;

	//FIXME: implement me
	assert( false );

    return ret;
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
    if( !m_modelNodeEditor)
    {
        m_modelNodeEditor = new ModelNodeEditor( shared_from_this() );
    }
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
            m_nodeLogic->Update( t );

        for( auto ch : m_children )
            ch->Update( t );
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
std::string                         BasicNode::SplitPrefix              ( std::string & str, const std::string & separator ) const
{
    assert( separator.length() == 1 );

	//strip unnecessary '/' 
	if( !str.empty() && str[ 0 ] == '/' )
		str.erase(0, 1);
	if( !str.empty() && str[ str.size() - 1 ] == '/' )
		str.erase(str.size() - 1);

    auto ret = Split( str, separator );

    if( ret.size() == 0 )
    {
        ret.push_back( "" );
    }
    else if ( ret.size() == 1 )
    {
        str = "";
    }
    else
    {
        str = Join( std::vector< std::string >( ret.begin() + 1, ret.end() ), separator );
    }

    return ret[ 0 ];
}

// ********************************
//
INodeLogicPtr                       BasicNode::GetLogic				    ()
{
    return m_nodeLogic;
}

} // model


namespace CloneViaSerialization {

// *******************************
//FIXME: name of method should indicate that timelines are modified or sth?
model::BasicNodePtr		CloneNode		( const model::BasicNode * obj, const std::string & prefix )
{
	{ obj; prefix; }

    //FIXME: implement me
	assert( false );
	
    return nullptr;
}

} //CloneViaSerialization


} // bv
