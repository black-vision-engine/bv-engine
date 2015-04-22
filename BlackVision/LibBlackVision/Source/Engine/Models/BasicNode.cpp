#include "BasicNode.h"

//FIXME: node na INode
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/BasicOverrideState.h"


namespace bv { namespace model {

// FIXME: hack
std::hash_map< IModelNode *, SceneNode * >    BasicNode::ms_nodesMapping;

namespace {

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
BasicNode::BasicNode( const std::string & name, ITimeEvaluatorPtr timeEvaluator, const PluginsManager * pluginsManager )
    : m_name( name )
    , m_pluginList( nullptr )
    , m_pluginsManager( pluginsManager )
    , m_overrideState( nullptr )
    , m_visible( true )
{
    if( pluginsManager == nullptr )
    {
        m_pluginsManager = &PluginsManager::DefaultInstance();
    }

    m_overrideState = new BasicOverrideState( timeEvaluator );
}

// ********************************
//
BasicNode::~BasicNode()
{
    delete m_overrideState;
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

    return std::make_shared<make_shared_enabler_BasicNode>( name, timeEvaluator, pluginsManager );
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

    if( name == "" || name == GetName() )
    {
        if( suffix.size() > 0 )
        {
            return GetNode( suffix, separator );
        }
        else
        {
            return shared_from_this();
        }
    }
    else
    {
        auto child = GetChild( name );

        if( child != nullptr )
        {
            return child->GetNode( suffix );
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
// FIXME: implement using API from BVScene
bool                            BasicNode::DeleteNode               ( const std::string & name, Renderer * renderer )
{
    auto node = GetChild( name );

    if( node )
    {
        auto basicNode = std::static_pointer_cast< BasicNode >( node );

        DetachChildNodeOnly( basicNode );
    
        basicNode->DeleteSelf( renderer );
    
        return true;
    }

    return false;
}

// ********************************
// FIXME: implement using API from BVScene
void                            BasicNode::AddChildNode             ( IModelNodePtr modelNode )
{
    // Verify validity
    assert( modelNode != nullptr );
    assert( ms_nodesMapping.find( modelNode.get() ) == ms_nodesMapping.end() );
    assert( ms_nodesMapping.find( this ) != ms_nodesMapping.end() );

    // Create engine node corresponding to modelNode
    BasicNode * basicModelNode = static_cast< BasicNode * >( modelNode.get() );
	{ basicModelNode; }
    /*
	SceneNode * engineNode = basicModelNode->BuildScene();

    // Register created node and its mapping
    ms_nodesMapping[ basicModelNode ] = engineNode;

    // Add model node to current tree along with corresponding engine node
    AddChildToModelOnly( std::static_pointer_cast< BasicNode >( modelNode ) );
    ms_nodesMapping[ this ]->AddChildNode( engineNode );
	*/
}

// ********************************
//
unsigned int                    BasicNode::GetNumChildren           () const
{
    return (unsigned int) m_children.size();
}

// ********************************
//
void                            BasicNode::EnableOverrideStateAM    ()
{
    m_overrideState->EnableAlphaAM();
}

// ********************************
//
void                            BasicNode::EnableOverrideStateNM    ()
{
    m_overrideState->EnableAlphaNM();
}

// ********************************
//
void                            BasicNode::DisableOverrideStateAM   ()
{
    m_overrideState->DisableAlphaAM();
}

// ********************************
//
void                            BasicNode::DisableOverrideStateNM   ()
{
    m_overrideState->DisableAlphaNM();
}

// ********************************
//
bool                            BasicNode::OverrideStateChangedAM   () const
{
    return m_overrideState->ChangedAM();
}

// ********************************
//
bool                            BasicNode::OverrideStateChangedNM   () const
{
    return m_overrideState->ChangedNM();
}

// ********************************
//
void                            BasicNode::SetOverrideStateChgAM    ( bool changed )
{
    return m_overrideState->SetChangedAM( changed );
}

// ********************************
//
void                            BasicNode::SetOverrideStateChgNM    ( bool changed )
{
    return m_overrideState->SetChangedNM( changed );
}

// ********************************
//
bool                            BasicNode::IsStateOverridenAM       () const
{
    return m_overrideState->IsAlphaEnabledAM();
}

// ********************************
//
bool                            BasicNode::IsStateOverridenNM       () const
{
    return m_overrideState->IsAlphaEnabledNM();
}

// ********************************
//
IOverrideState *                BasicNode::GetOverrideState         ()
{
    return m_overrideState;
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
// FIXME: implement using API from BVScene
void            BasicNode::DeleteSelf                       ( Renderer * renderer )
{
	{ renderer; }
	/*
    // Unregister updater
    UpdatersManager::Get().RemoveNodeUpdater( this );


    // Remove engine node and clear all resources
    auto engineNode = ms_nodesMapping[ this ];
    ms_nodesMapping.erase( this );

    // Clear all OpenGL resources
    SceneNode::DeleteNode( engineNode, renderer );

    // Remove all children
    for( auto ch : m_children )
    {
        ch->DeleteSelf( renderer ); 
    }
	*/
}

// ********************************
//
void            BasicNode::SetPlugins                       ( DefaultPluginListFinalizedPtr plugins )
{
    m_pluginList = plugins;
}

// ********************************
//
void             BasicNode::NonNullPluginsListGuard ()
{
    if( !m_pluginList )
    {
        m_pluginList = std::make_shared< DefaultPluginListFinalized >();
    }
}

// ********************************
//
bool            BasicNode::AddPlugin                        ( IPluginPtr plugin )
{
    NonNullPluginsListGuard();

    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    assert( m_pluginsManager->CanBeAttachedTo( plugin->GetTypeUid(), prev ) );

    if( !m_pluginsManager->CanBeAttachedTo( plugin->GetTypeUid(), prev ) )
    {
        return false;
    }

    m_pluginList->AttachPlugin( plugin );

    return true;
}

// ********************************
//
bool            BasicNode::AddPlugin                        ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator )
{
    NonNullPluginsListGuard ();

    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    if( !m_pluginsManager->CanBeAttachedTo( uid, prev ) )
    {
		throw ( uid + " cannot be attached to " + prev->GetTypeUid() ); //FIXME: we do not use exceptions !!!!
    }

    m_pluginList->AttachPlugin( m_pluginsManager->CreatePlugin( uid, prev, timeEvaluator ) );

    return true;
}

// ********************************
//
bool            BasicNode::AddPlugin                    ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    NonNullPluginsListGuard ();

    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    if( !m_pluginsManager->CanBeAttachedTo( uid, prev ) )
    {
        return false;
    }

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
bool           BasicNode::AddPlugins              ( const std::vector< std::string > & uids, const std::vector< std::string > & names, ITimeEvaluatorPtr timeEvaluator )
{
    if( uids.size() != names.size() )
    {
        return false;
    }

    for( unsigned int i = 0; i < names.size(); ++i )
    {
        if( !AddPlugin( uids[ i ], names[ i ], timeEvaluator ) )
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
}

// ********************************
//
void BasicNode::Update( TimeType t )
{
    if( IsVisible() )
    {
        m_overrideState->Update( t );

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

    std::vector< std::string > ret;

    //FIXME: _SCL_SECURE_NO_WARNINGS is defined due to this fuckin line (or fuckin VS)
    boost::split( ret, str, boost::is_any_of( separator ) );

    if( ret.size() == 0 )
    {
        return "";
    }
    else if ( ret.size() == 1 )
    {
        str = "";

        return ret[ 0 ];
    }

    str = boost::algorithm::join( std::vector< std::string >( ret.begin() + 1, ret.end() ), separator );

    return ret[ 0 ];
}

} // model
} // bv
