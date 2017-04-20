#include "stdafx.h"

#include "ModelState.h"

#include "BVProject.h"

#include "BasicNode.h"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model 
{
// ********************************
//
struct NodeState
{
    const IModelNode *  parent;
    const SceneModel*   scene;
    std::string         path;

    NodeState( const IModelNode * parent, const SceneModel* scene, const std::string & path )
        : parent( parent )
        , scene( scene )
        , path( path )
    {}
};

// ********************************
//
std::string                         ModelState::QueryNodeSceneName  ( const IModelNode * node ) const
{
    if( auto ns = GetNodeState( node ) )
    {
        return ns->scene->GetName();
    }
    else
    {
        return "";
    }
}

// ***********************
//
const SceneModel *                  ModelState::QueryNodeScene  ( const IModelNode * node ) const
{
    if( auto ns = GetNodeState( node ) )
    {
        return ns->scene;
    }
    else
    {
        return nullptr;
    }
}

// ********************************
//
const IModelNode *                  ModelState::QueryNodeParent ( const IModelNode * node ) const
{
    if( auto ns = GetNodeState( node ) )
    {
        return ns->parent;
    }
    else
    {
        return nullptr;
    }
}

// ********************************
//
std::string                         ModelState::QueryNodePath   (  const IModelNode * node ) const
{
    if( auto ns = GetNodeState( node ) )
    {
        return ns->path;
    }
    else
    {
        return "";
    }
}

// ***********************
//
std::string                         ModelState::BuildIndexPath  ( const IModelNode * node )
{
    auto parent = static_cast< const BasicNode * >( QueryNodeParent( node ) );
    if( parent == nullptr )
        return "#0";

    for( UInt32 i = 0; i < parent->GetNumChildren(); ++i )
    {
        if( parent->GetChild( i ) == node )
        {
            std::string indexPath = BuildIndexPath( parent ) +  "/#" + SerializationHelper::T2String( i );
            return indexPath;
        }
    }

    assert( false );
    return "";
}

// ***********************
//
UInt32                              ModelState::GetNodeIndex    ( const IModelNode * node ) const
{
    auto parent = static_cast< const BasicNode * >( QueryNodeParent( node ) );
    if( parent == nullptr )
        return std::numeric_limits< UInt32 >::max();

    for( UInt32 i = 0; i < parent->GetNumChildren(); ++i )
    {
        if( parent->GetChild( i ) == node )
        {
            return i;
        }
    }

    return std::numeric_limits< UInt32 >::max();
}

// ********************************
//
namespace
{

SceneModelConstPtr IsSceneRootNode    ( const IModelNode * node, const BVProject * project )
{
    auto scenesModels = project->GetModelScenes();

    for( auto sm : scenesModels )
    {
        if( sm->GetRootNode().get() == node )
        {
            return sm;
        }
    }
    
    return nullptr;
}


} // anonymous


// ********************************
//
bool                                ModelState::RegisterNode    ( const IModelNode * node, const IModelNode * parent )
{
    if( parent != nullptr )
    {
        if( m_nodeStates.find( parent ) == m_nodeStates.end() )
        {
            RegisterNode( parent, nullptr );
    //        if( parent->GetName() == "main root" )
    //        {
    //            return true;
    //        }

    //        assert( !"Parent must be registered" );
    //        return false;
        }
    }

    if( m_nodeStates.find( node ) != m_nodeStates.end() )
    {
        if( !UnregisterNode( node ) )
        {
            return false;
        }
    }

    auto sceneParent = IsSceneRootNode( parent, m_project );

    if( sceneParent != nullptr )
    {
        m_nodeStates[ node ] = new NodeState( parent, sceneParent.get(), parent->GetName() + "/" + node->GetName() );
    }
    else
    {
        auto sceneModel = IsSceneRootNode( node, m_project );
        if( sceneModel != nullptr )
        {
            m_nodeStates[ node ] = new NodeState( nullptr, sceneModel.get(), node->GetName() );
            //if( parent != nullptr )
            //{
            //    assert( !"Scene root node cannnot have parent" );
            //    return false;
            //}
        }
        else
        {
            std::string parentPath = QueryNodePath( parent );
            auto parentScene = QueryNodeScene( parent );

            auto path = parentPath.empty() ? node->GetName() : parentPath + "/" + node->GetName();

            m_nodeStates[ node ] = new NodeState( parent, parentScene, path );
        }
    }

    assert( dynamic_cast< const BasicNode * >( node ) );
    auto basicNode = static_cast< const BasicNode * >( node );

    bool result = true;

    for( UInt32 i = 0; i < basicNode->GetNumChildren(); ++i )
    {
        result = result && RegisterNode( basicNode->GetChild( i ), basicNode );
    }

    return result;
}

// ********************************
//
bool                                ModelState::UnregisterNode  ( const IModelNode * node )
{
    if( m_nodeStates.find( node ) != m_nodeStates.end() )
    {
        delete m_nodeStates[ node ];
        m_nodeStates.erase( node );

        bool result = true;

        assert( dynamic_cast< const BasicNode * >( node ) );
        auto basicNode = static_cast< const BasicNode * >( node );

        for( UInt32 i = 0; i < basicNode->GetNumChildren(); ++i )
        {
            result = result && UnregisterNode( basicNode->GetChild( i ) );
        }

        return true;
    }
    else
    {
        return false;
    }
}
// ********************************
//
ModelState &                        ModelState::GetInstance     ()
{
    static auto instance = ModelState();
    return instance;
}

// ********************************
//
bool                                ModelState::RegisterBVProject ( const BVProject * project )
{
    if( m_project == nullptr )
    {
        m_project = project;
        return true;
    }
    else
    {
        assert( !"Project already registered" );
        return false;
    }
}

// ********************************
//
ModelState::ModelState()
    : m_project( nullptr )
{}

// ********************************
//
ModelState::~ModelState()
{
    for( auto & ns : m_nodeStates )
    {
        delete ns.second;
        ns.second = nullptr;
    }
}

// ********************************
//
std::string                         ModelState::RestoreNodePath ( const IModelNode * node, std::string * sceneName ) const
{
    SceneModelConstPtr scene = nullptr;

    const IModelNode * tmpNode = node;

    std::string path = "";

    for(; tmpNode != nullptr;)
    {
        scene = IsSceneRootNode( tmpNode, m_project );
        if( scene != nullptr )
        {
            path = scene->GetName() + "/" + path;
            *sceneName = scene->GetName();
            break;
        }
        else
        {
            path = tmpNode->GetName() + "/" + path;
            tmpNode = QueryNodeParent( tmpNode );
        }
    }

    return path;
}

// ********************************
//
const NodeState *                   ModelState::GetNodeState    ( const IModelNode * node ) const
{
    auto it = m_nodeStates.find( node );
    if( it != m_nodeStates.end() )
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

// ========================================================================= //
// Node selection
// ========================================================================= //

// ***********************
//
bool    ModelState::IsSelected      ( IModelNodeConstPtr node )
{
    auto iter = m_selectedNodes.find( node );
    if( iter == m_selectedNodes.end() )
        return false;

    return true;
}

// ***********************
//
void    ModelState::Select          ( IModelNodePtr node, glm::vec4 color )
{
    m_selectedNodes[ node ] = color;
}

// ***********************
//
bool    ModelState::Unselect        ( IModelNodePtr node )
{
    auto iter = m_selectedNodes.find( node );
    
    if( iter == m_selectedNodes.end() )
        return false;

    m_selectedNodes.erase( iter );
    return true;
}

void    ModelState::UnselectRecursive   ( IModelNodePtr node )
{
    Unselect( node );

    for( unsigned int i = 0; i < node->GetNumChildren(); ++i )
    {
        auto basicNode = std::static_pointer_cast< BasicNode >( node );
        UnselectRecursive( basicNode->GetChild( i ) );
    }
}

// ***********************
//
void    ModelState::UnselectAll     ()
{
    m_selectedNodes.clear();
}

// ***********************
//
std::set< IModelNodeConstPtr >         ModelState::GetSelectedNodes()
{
    std::set< IModelNodeConstPtr > ret;

    for( auto & node : m_selectedNodes )
        ret.insert( node.first );

    return ret;
}

// ***********************
//
const BVProject *                   ModelState::GetBVProject        () const
{
    return m_project;
}

// ***********************
//
glm::vec4                           ModelState::GetSelectedNodeColor ( IModelNodeConstPtr node )
{
    return m_selectedNodes[ node ];
}


} // model
} // bv