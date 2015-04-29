#include "BVSceneEditor.h"

#include "Engine/Models/BVScene.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVSceneTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// *******************************
//
BVSceneEditor::BVSceneEditor                ( BVScene * scene )
    : m_pScene( scene )
    , m_detachedModelNode( nullptr )
    , m_detachedSceneNode( nullptr )
{
}

// *******************************
//
void    BVSceneEditor::SetRootNode          ( model::IModelNodePtr rootNode )
{
    if( S()->m_pModelSceneRoot != rootNode )
    {
        if( S()->m_pModelSceneRoot != nullptr )
        {
            DeleteRootNode();
        }

        auto rootNodeImpl   = std::static_pointer_cast< model::BasicNode >( rootNode );
        auto engineNode     = BVSceneTools::BuildEngineSceneNode( rootNodeImpl, m_nodesMapping );

        S()->m_pModelSceneRoot  = rootNodeImpl;
        S()->m_pEngineSceneRoot = engineNode;
    }
}

// *******************************
//
bool    BVSceneEditor::DeleteRootNode       ()
{
    auto nodesPair = DetachRootNodes();
    
    if( nodesPair.first )
    {
        DeleteDetachedNodes( nodesPair.first, nodesPair.second );

        return true;
    }

    return false;
}

// *******************************
//
void    BVSceneEditor::AddChildNode         ( model::IModelNodePtr parentNode, model::IModelNodePtr childNode )
{
    auto modelParentNode    = std::static_pointer_cast< model::BasicNode >( parentNode );
    auto modelChildNode     = std::static_pointer_cast< model::BasicNode >( childNode );

    if( modelParentNode && modelChildNode )
    {
        auto engineParent = GetEngineNode( modelParentNode );
        auto engineChild = BVSceneTools::BuildEngineSceneNode( modelChildNode, m_nodesMapping );
    
        auto parentPair = TNodesPair( modelParentNode, engineParent );
        auto childPair  = TNodesPair( modelChildNode, engineChild );
        
        AttachNodes( parentPair, childPair );
    }
}

// *******************************
//
bool    BVSceneEditor::DeleteChildNode      ( model::IModelNodePtr parentNode, const std::string & childNodeName )
{
    auto parentModelNode  = std::static_pointer_cast< model::BasicNode >( parentNode );
    
    if( parentModelNode )
    {
        auto modelChildNode = std::static_pointer_cast< model::BasicNode >( parentModelNode->GetChild( childNodeName ) );

        if( modelChildNode )
        {
            auto engineChildNode = GetEngineNode( modelChildNode );
            assert( engineChildNode != nullptr );

            DetachChildNode( parentModelNode, modelChildNode );
            DeleteDetachedNodes( modelChildNode, engineChildNode );

            return true;
        }
    }

    return false;
}

// *******************************
//
void    BVSceneEditor::AttachRootNode      ()
{
    if( S()->m_pModelSceneRoot )
    {
        DeleteRootNode();
    }

    auto nodesPair = TNodesPair( m_detachedModelNode, m_detachedSceneNode );

    AttachRootNodes( nodesPair );

    m_detachedModelNode = nullptr;
    m_detachedSceneNode = nullptr;
}

// *******************************
//
bool    BVSceneEditor::DetachRootNode      ()
{
    if( m_detachedModelNode )
    {
        DeleteDetachedNodes( m_detachedModelNode, m_detachedSceneNode );
    }

    m_detachedModelNode = std::static_pointer_cast< model::BasicNode >( S()->m_pModelSceneRoot );
    m_detachedSceneNode = S()->m_pEngineSceneRoot;

    S()->m_pModelSceneRoot  = nullptr;
    S()->m_pEngineSceneRoot = nullptr;

    return m_detachedModelNode != nullptr;
}

// *******************************
//
bool                    BVSceneEditor::AttachChildNode     ( model::IModelNodePtr parent )
{
    if( m_detachedModelNode != nullptr && parent != nullptr )
    {
        auto modelParent    = std::static_pointer_cast< model::BasicNode >( parent );
        auto engineParent   = GetEngineNode( modelParent );
        assert( engineParent );

        auto parentPair = TNodesPair( modelParent, engineParent );
        auto childPair  = TNodesPair( m_detachedModelNode, m_detachedSceneNode );

        AttachNodes( parentPair, childPair );

        m_detachedModelNode = nullptr;
        m_detachedSceneNode = nullptr;

        return true;
    }

    return false;
}

// *******************************
//
bool                    BVSceneEditor::DetachChildNode     ( model::IModelNodePtr parent, const std::string & nodeToDetach )
{
    if( m_detachedModelNode )
    {
        DeleteDetachedNodes( m_detachedModelNode, m_detachedSceneNode );
    }

    auto modelParent = std::static_pointer_cast< model::BasicNode >( parent );

    if( modelParent )
    {
        auto modelChild = std::static_pointer_cast< model::BasicNode >( modelParent->GetChild( nodeToDetach ) );

        // FIXME: implement
        //auto engineParent = GetEngineNode( modelParent );
        //assert( engineParent != nullptr );

        //auto engineChild = GetEngineNode( modelChild );
        //assert( engineChild != nullptr );

        //auto parentPair = TNodesPair( modelParent, engineParent );
        //auto childPair  = TNodesPair( m_detachedModelNode, m_detachedSceneNode );

        //DetachNodes( parentPair, childPair );

        return true;
    }

    return false;
}

// *******************************
//
model::IModelNodePtr    BVSceneEditor::GetDetachedNode      ()
{
    return m_detachedModelNode;
}

// *******************************
//
void            BVSceneEditor::DeleteDetachedNode           ()
{
    if( m_detachedModelNode )
    {
        assert( m_detachedSceneNode );

        DeleteDetachedNodes( m_detachedModelNode, m_detachedSceneNode );
    }
}

// *******************************
//
model::IModelNodePtr        BVSceneEditor::GetRootNode      ()
{
    return S()->m_pModelSceneRoot;
}

// *******************************
//
void                        BVSceneEditor::AttachRootNodes  ( TNodesPair & nodesPair )
{
    assert( S()->m_pModelSceneRoot == nullptr );
    assert( S()->m_pEngineSceneRoot == nullptr );

    S()->m_pModelSceneRoot = nodesPair.first;
    S()->m_pEngineSceneRoot = nodesPair.second;
}

// *******************************
//
BVSceneEditor::TNodesPair   BVSceneEditor::DetachRootNodes  ()
{
    auto modelNode   = std::static_pointer_cast< model::BasicNode >( S()->m_pModelSceneRoot );
    auto engineNode  = S()->m_pEngineSceneRoot;

    S()->m_pModelSceneRoot  = nullptr;
    S()->m_pEngineSceneRoot = nullptr;

    return TNodesPair( modelNode, engineNode );
}

// *******************************
//
BVSceneEditor::TNodesPair   BVSceneEditor::DetachNodes      ( model::BasicNodePtr parentNode, const std::string & childNodeName )
{
    model::BasicNodePtr modelNode = nullptr;
    SceneNode * engineNode = nullptr;

    if( parentNode )
    {
        auto childNode = std::static_pointer_cast< model::BasicNode >( parentNode->GetChild( childNodeName ) );

        if( childNode )
        {
            auto engineParent = GetEngineNode( parentNode );
            assert( engineParent != nullptr );
            
            auto engineChild = GetEngineNode( childNode );
            assert( engineChild != nullptr );

            parentNode->DetachChildNodeOnly( childNode );
            engineParent->DetachChildNode( engineChild );

            modelNode   = childNode;
            engineNode  = engineChild;
        }
    }

    return TNodesPair( modelNode, engineNode );
}

// *******************************
//
void            BVSceneEditor::AttachNodes          ( TNodesPair & parentPair, TNodesPair & childPair )
{
    parentPair.first->AddChildToModelOnly( childPair.first );
    parentPair.second->AddChildNode( childPair.second );
}

// *******************************
//
SceneNode *     BVSceneEditor::GetEngineNode        ( model::BasicNodePtr node )
{
    return m_nodesMapping[ node.get() ];
}

// *******************************
//
void            BVSceneEditor::UnregisterUpdaters   ( model::BasicNodePtr node )
{
    UpdatersManager::Get().RemoveNodeUpdater( node.get() );

    for( unsigned int i = 0; i < node->GetNumChildren(); ++i )
    {
        auto child = node->GetChild( i );

        UnregisterUpdaters( child );
    }
}

// *******************************
//
void            BVSceneEditor::RemoveNodeMappings   ( model::BasicNodePtr node )
{
    assert( m_nodesMapping.find( node.get() ) != m_nodesMapping.end() );

    m_nodesMapping.erase( node.get() );
    
    for( unsigned int i = 0; i < node->GetNumChildren(); ++i )
    {
        auto child = node->GetChild( i );

        RemoveNodeMappings( child );
    }
}

// *******************************
//
void            BVSceneEditor::DetachChildNode      ( model::BasicNodePtr parentNode, model::BasicNodePtr childNode )
{
    auto engineParent = GetEngineNode( parentNode );
    auto engineChild  = GetEngineNode( childNode );

    assert( engineParent );
    assert( engineChild );

    parentNode->DetachChildNodeOnly( childNode );
    engineParent->DetachChildNode( engineChild );
}

// *******************************
//
void            BVSceneEditor::DeleteDetachedNodes  ( model::BasicNodePtr modelNode, SceneNode * engineNode )
{
    UnregisterUpdaters( modelNode );
    RemoveNodeMappings( modelNode );

    SceneNode::DeleteNode( engineNode, S()-> m_renderer );
}

} //bv
