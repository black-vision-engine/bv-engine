#include "stdafx.h"

#include "ModelState.h"

#include "BVProject.h"

#include "BasicNode.h"

namespace bv { namespace model 
{
// ********************************
//
struct NodeState
{
    const IModelNode *  parent;
    std::string         sceneName;
    std::string         path;

    NodeState( const IModelNode * parent, const std::string & sceneName, const std::string & path )
        : parent( parent )
        , sceneName( sceneName )
        , path( path )
    {}
};

// ********************************
//
const std::string &                 ModelState::QueryNodeScene  ( const IModelNode * node ) const
{
    return m_nodeStates.at( node )->sceneName;
}

// ********************************
//
const IModelNode *                  ModelState::QueryNodeParent ( const IModelNode * node ) const
{
    return m_nodeStates.at( node )->parent;
}

// ********************************
//
const std::string &                 ModelState::QueryNodePath   (  const IModelNode * node ) const
{
    return m_nodeStates.at( node )->path;
}

// ********************************
//
namespace
{

SceneModelConstPtr IsSceneRootNode    ( const IModelNode * node, const BVProject * project )
{
    auto scenesModels = project->GetScenes();

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
            if( parent->GetName() == "main root" )
            {
                return true;
            }

            assert( !"Parent must be registered" );
            return false;
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
        m_nodeStates[ node ] = new NodeState( parent, sceneParent->GetName(), sceneParent->GetName() + "/" + node->GetName() );
    }
    else
    {
        auto sceneNode = IsSceneRootNode( node, m_project );
        if( sceneNode != nullptr )
        {
            m_nodeStates[ node ] = new NodeState( nullptr, sceneNode->GetName(), sceneNode->GetName() );
            if( parent != nullptr )
            {
                assert( !"Scene root node cannnot have parent" );
                return false;
            }
        }
        else
        {
            std::string parentPath = QueryNodePath( parent );
            std::string parentScene = QueryNodeScene( parent );

            m_nodeStates[ node ] = new NodeState( parent, parentScene, parentPath + "/" + node->GetName() );
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

// ========================================================================= //
// Node selection
// ========================================================================= //

// ***********************
//
bool    ModelState::IsSelected      ( IModelNodePtr node )
{
    auto iter = m_selectedNodes.find( node );
    if( iter == m_selectedNodes.end() )
        return false;

    return true;
}

// ***********************
//
void    ModelState::Select          ( IModelNodePtr node )
{
    m_selectedNodes.insert( node );
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

// ***********************
//
void    ModelState::UnselectAll     ()
{
    m_selectedNodes.clear();
}

// ***********************
//
std::set< IModelNodePtr > &         ModelState::GetSelectedNodes()
{
    return m_selectedNodes;
}

} // model
} // bv