#include "stdafx.h"

#include "BasicNode.h"

#include <set>

#include "Engine/Models/Builder/NodeLogicHolder.h"

//FIXME: node na INode
#include "Tools/StringHeplers.h"

#include "Engine/Models/Plugins/Manager/PluginsManager.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"

#include "Engine/Models/ModelNodeEditor.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/SceneModel.h"

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Timeline/TimelineHelper.h"

#include "Engine/Models/NodeEffects/ModelNodeEffect.h"

#include "Tools/PrefixHelper.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVSerializeContext.h"
//#include "Serialization/SerializationObjects.inl"
#include "Serialization/BV/CloneViaSerialization.h"
#include "Engine/Models/Plugins/Channels/HelperPixelShaderChannel.h"
#include "Assets/AssetDescsWithUIDs.h"

#include "UseLoggerLibBlackVision.h"

#include "ModelState.h"

#include "Engine/Models/BoundingVolume.h"



#include "Memory/MemoryLeaks.h"



namespace bv { 
    
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
    auto bv = new BoundingVolume( nullptr, nullptr );
    m_boundingVolume = BoundingVolumePtr( bv );

    if( pluginsManager == nullptr )
    {
        m_pluginsManager = &PluginsManager::DefaultInstance();
    }
}

// ********************************
//
BasicNode::~BasicNode()
{
    if( m_nodeLogic )
        m_nodeLogic->Deinitialize();

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

    //if( context->detailedInfo )
    //{
    //    GetAssetsWithUIDs( *context->GetAssets(), this );
    //}

    if( context->pluginsInfo )
    {
        ser.EnterArray( "plugins" );
            for( unsigned int  i = 0; i < m_pluginList->NumPlugins(); i++ )
            {
                auto plugin_ = m_pluginList->GetPlugin( i );
                auto plugin = std::static_pointer_cast< BasePlugin >( plugin_ );
                assert( plugin );
                plugin->Serialize( ser );
            }
        ser.ExitChild(); // plugins
    }

    if( m_modelNodeEffect )
        m_modelNodeEffect->Serialize( ser );

    if( m_nodeLogic )  // Logic chooses by itself, what to write as detailed info.
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
BasicNodePtr BasicNode::Create( const IDeserializer& deser )
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
    auto node = BasicNode::Create( name, nullptr );

    node->m_visible = deser.GetAttribute( "visible" ) == "false" ? false : true;

// plugins
    deserContext->ClearRendererContextes();
    auto plugins = SerializationHelper::DeserializeArray< BasePlugin >( deser, "plugins" );

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
    
    if( plugins.size() > 0 )
    {
        auto psc = plugins.back()->GetPixelShaderChannel();

        if( psc )
        {
            HelperPixelShaderChannel::SetRendererContextUpdate( psc );
        }
    }

// node effect
    if( deser.EnterChild( "effect" ) )
    {
        auto effect = ModelNodeEffect::CreateTyped( deser );
        
        if( effect != nullptr)
        {
            node->SetNodeEffect( effect );
        }
        else
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "node " << name << " cannot deserialize node effect.";
        }

        deser.ExitChild();  // effect
    }

// children
    auto children = SerializationHelper::DeserializeArray< BasicNode >( deser, "nodes" );

    for( auto child : children )
    {
        node->AddChildToModelOnly( child );
    }

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
BasicNodePtr					BasicNode::Clone			() const
{
    auto assets = std::make_shared< AssetDescsWithUIDs >();
    //FIXME: const hack
    GetAssetsWithUIDs( *assets, this, true ); // FIXME: Not needed any more. assets are stored in serialization context.

    OffsetTimeEvaluatorPtr sceneTimeline;
    
    auto scene = ModelState::GetInstance().QueryNodeScene( this );
    if( scene )
    {
        auto timeline = TimelineManager::GetInstance()->GetTimeEvaluator( scene->GetName() );
        sceneTimeline = std::static_pointer_cast< OffsetTimeEvaluator >( timeline );
    }

    return BasicNodePtr( CloneViaSerialization::Clone( this, "node", assets, sceneTimeline ) );
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
        if( childIdx < ( Int32 )m_children.size() )
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

    auto nodeLogic = GetLogic();
    if( nodeLogic )
    {
        auto params =  nodeLogic->GetParameters();
        ret.insert( ret.end(), params.begin(), params.end() );
    }

    return ret;
}


// ********************************
//
std::vector< ITimeEvaluatorPtr >    BasicNode::GetTimelines			( bool recursive ) const
{
    std::set< ITimeEvaluatorPtr > timelines;

    auto params = GetParameters();

    for( auto param : params )
    {
        timelines.insert( param->GetTimeEvaluator() );
    }

    auto plugins = GetPluginList();
    for( UInt32 i = 0; i < plugins->NumPlugins(); i++ )
    {
        auto pluginParamValModel = plugins->GetPlugin( i )->GetPluginParamValModel();
        if( pluginParamValModel )
        {
            timelines.insert( pluginParamValModel->GetTimeEvaluator() );
        }
    }

    if( recursive )
    {
        for( auto child : m_children )
        {
            auto ts = child->GetTimelines( true );
            timelines.insert( ts.begin(), ts.end() ); // FIXME: remove duplicates
        }
    }

    return std::vector< ITimeEvaluatorPtr >( timelines.begin(), timelines.end() );
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

    auto trans = m_pluginList->GetFinalizePlugin()->GetParamTransform()->Evaluate();

    auto plRect = m_pluginList->GetFinalizePlugin()->GetAABB( trans );

    if( plRect )
    {
        r.Include( *plRect );
    }

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

    auto trans = parentTransformation * m_pluginList->GetFinalizePlugin()->GetParamTransform()->Evaluate();

    auto plRect = m_pluginList->GetFinalizePlugin()->GetAABB( trans );

    if( plRect )
        r.Include( *plRect );


    for( auto ch : m_children )
    {
        r.Include( ch->GetAABB( trans ) );
    }

    return r;
}

//// ********************************
////
//BoundingVolume 						    BasicNode::GetBoundingVolume		( const glm::mat4 & /*parentTransformation*/ ) const
//{
//    assert( false );
//    return BoundingVolume( mathematics::Box() );
//}

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
    AddChildToModelOnly( n, ( UInt32 ) m_children.size() );
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

    ModelState::GetInstance().RegisterNode( n.get(), this );
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

            ModelState::GetInstance().UnregisterNode( n.get() );

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

namespace {

BoundingVolumePtr CreateBoundingVolume( DefaultPluginListFinalizedPtr pluginList )
{
    auto vac_ = pluginList->GetFinalizePlugin()->GetVertexAttributesChannel();
    auto param = pluginList->GetFinalizePlugin()->GetParamTransform();

    auto vac = ( vac_ ) ? Cast< VertexAttributesChannel * >( RemoveConst( vac_.get() ) ) : nullptr;

    return std::make_shared< BoundingVolume >( vac, param.get() );
}

}

// ********************************
//
bool            BasicNode::AddPlugin                        ( IPluginPtr plugin )
{
    m_pluginList->AttachPlugin( plugin );
	RecreateBoundingVolume();

    return true;
}

// ********************************
//
void			BasicNode::RecreateBoundingVolume			()
{
	m_boundingVolume = CreateBoundingVolume( m_pluginList );
}

// ********************************
//
bool            BasicNode::AddPlugin                        ( const std::string & uid, ITimeEvaluatorPtr timeEvaluator )
{
    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    auto plugin = m_pluginsManager->CreatePlugin( uid, prev, timeEvaluator );
    if( plugin )
    {
        m_pluginList->AttachPlugin( plugin );
        RecreateBoundingVolume();
    
        return true;
    }
 
    return false;
}

// ********************************
//
bool            BasicNode::AddPlugin                    ( const std::string & uid, const std::string & name, ITimeEvaluatorPtr timeEvaluator )
{
    IPluginPtr prev = m_pluginList->NumPlugins() > 0 ? m_pluginList->GetLastPlugin() : nullptr;

    m_pluginList->AttachPlugin( m_pluginsManager->CreatePlugin( uid, name, prev, timeEvaluator ) );
	RecreateBoundingVolume();

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
    RemoveLogic();

    m_nodeLogic = logic;
    m_nodeLogic->Initialize();
}

// ***********************
//
void            BasicNode::RemoveLogic              ()
{
    if( m_nodeLogic )
        m_nodeLogic->Deinitialize();
    m_nodeLogic = nullptr;
}

// ********************************
//
void BasicNode::Update( TimeType t )
{
    if( IsVisible() )
    {
        if( m_nodeLogic )
        {
            m_nodeLogic->PreNodeUpdate( t );
        }

        if ( m_modelNodeEffect )
        {
            m_modelNodeEffect->Update( t );
        }

        m_pluginList->Update( t );

        if( m_boundingVolume )
        {
            m_boundingVolume->UpdateOwnBox( m_pluginList->GetFinalizePlugin()->GetVertexAttributesChannel() );
        }

        if( m_nodeLogic )
        {
            m_nodeLogic->Update( t );
        }

        for( auto ch : m_children )
        {
            ch->Update( t );
        }

        m_boundingVolume->IncludeChildrenBox( GetBoundingBoxRecursive() );

        if( m_nodeLogic )
        {
            m_nodeLogic->PostChildrenUpdate( t );
        }
    }
}

// ********************************
//
BoundingVolumeConstPtr              BasicNode::GetBoundingVolume		    () const
{
    return m_boundingVolume;
}

// ********************************
//
mathematics::Box                    BasicNode::GetBoundingBoxRecursive		() const
{
    mathematics::Box ret;

    if( m_boundingVolume->GetBoundingBox() )
    {
        ret.Include(  *m_boundingVolume->GetBoundingBox() );

        for( UInt32 i = 0; i < m_children.size(); ++i )
        {
            const glm::mat4 & transform = m_children[ i ]->GetFinalizePlugin()->GetTransformChannel()->GetTransformValue()->GetValue();
            
            mathematics::Box childBox = m_children[ i ]->GetBoundingBoxRecursive();
            childBox.Transform( transform );
            
            ret.Include( childBox );
        }
    }

    return ret;
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
        bool success = ( !( std::stringstream( str.substr( 1, str.length() ) ) >> result ).fail() );
        if( success )
        {
            return result;
        }
    }

    return -1;
}

} // model


namespace CloneViaSerialization {

// *******************************
//
void                    UpdateTimelines ( model::BasicNode * obj, const std::string & prefix, const std::string & destScene, bool recursive )
{
    for( auto param : obj->GetParameters() )
    {
        if( param->GetTimeEvaluator() )
        {
            auto timelinePath = model::TimelineHelper::CombineTimelinePath( destScene, prefix + param->GetTimeEvaluator()->GetName() );
            auto timeline = model::TimelineManager::GetInstance()->GetTimeEvaluator( timelinePath );
            param->SetTimeEvaluator( timeline );
        }
    }

    auto plugins = obj->GetPluginList();
    for( UInt32 i = 0; i < plugins->NumPlugins(); i++ )
    {
        auto pluginModel = plugins->GetPlugin( i )->GetPluginParamValModel();
        if( pluginModel && pluginModel->GetTimeEvaluator() )
        {
            auto timelinePath = model::TimelineHelper::CombineTimelinePath( destScene, prefix + pluginModel->GetTimeEvaluator()->GetName() );
            auto timeline = model::TimelineManager::GetInstance()->GetTimeEvaluator( timelinePath );
            //FIXME: cast
            std::static_pointer_cast< model::DefaultPluginParamValModel >( pluginModel )->SetTimeEvaluator( timeline );
        }
    }

    if( recursive )
    {
        for( unsigned int i = 0; i < obj->GetNumChildren(); i++ )
        {
            UpdateTimelines( obj->GetChild( i ).get(), prefix, destScene, true );
        }
    }
}

} //CloneViaSerialization

} // bv
