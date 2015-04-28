#include "BVSceneEditor.h"

#include "Engine/Models/BVScene.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVSceneTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// *******************************
//
BVSceneEditor::BVSceneEditor                        ( BVScene * scene )
    : m_pScene( scene )
    , m_detachedModelNode( nullptr )
    , m_detachedSceneNode( nullptr )
{
}

// *******************************
//
void    BVSceneEditor::BuildScene           ()
{
    assert( m_nodesMapping.size() == 0 );
    assert( S()->m_pModelSceneRoot != nullptr );
    assert( S()->m_pEngineSceneRoot == nullptr );

    auto dummy = S()->m_pModelSceneRoot;
    S()->m_pModelSceneRoot = nullptr;

    SetRootNode( dummy );
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
    auto modelRoot  = std::static_pointer_cast< model::BasicNode >( S()->m_pModelSceneRoot );
    auto engineRoot = S()->m_pEngineSceneRoot;

    if( modelRoot )
    {
        assert( engineRoot );

        S()->m_pModelSceneRoot  = nullptr;
        S()->m_pEngineSceneRoot = nullptr;

        DeleteDetachedNodes( modelRoot, engineRoot );
    }

    return modelRoot != nullptr;
}

// *******************************
//
void    BVSceneEditor::AttachRootNode      ( model::IModelNodePtr rootNode )
{
    if( S()->m_pModelSceneRoot && S()->m_pModelSceneRoot != rootNode )
    {
        DeleteRootNode();
    }

    S()->m_pModelSceneRoot = std::static_pointer_cast< model::BasicNode >( rootNode );
}

// *******************************
//
model::IModelNodePtr    BVSceneEditor::DetachRootNode      ()
{
    if( m_detachedModelNode )
    {
        DeleteDetachedNodes( m_detachedModelNode, m_detachedSceneNode );
    }

    m_detachedModelNode  = std::static_pointer_cast< model::BasicNode >( S()->m_pModelSceneRoot );
    m_detachedSceneNode = S()->m_pEngineSceneRoot;

    S()->m_pModelSceneRoot = nullptr;
    S()->m_pEngineSceneRoot = nullptr;

    return m_detachedModelNode;
}

// *******************************
//
void    BVSceneEditor::AddChildNode         ( model::IModelNodePtr parentNode, model::IModelNodePtr childNode )
{
    auto parentNodeImpl = std::static_pointer_cast< model::BasicNode >( parentNode );
    auto childNodeImpl  = std::static_pointer_cast< model::BasicNode >( childNode );
    
    if( parentNodeImpl && childNodeImpl )
    {
        AttachChildNode( parentNode, childNode );

        auto engineParent = GetEngineNode( parentNodeImpl );
        auto engineChild = BVSceneTools::BuildEngineSceneNode( childNodeImpl, m_nodesMapping );
    
        engineParent->AddChildNode( engineChild );
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
void                    BVSceneEditor::AttachChildNode     ( model::IModelNodePtr parent, model::IModelNodePtr nodeToAttach )
{
    if( parent && nodeToAttach )
    {
        auto parentImpl = std::static_pointer_cast< model::BasicNode >( parent );
        auto childImpl = std::static_pointer_cast< model::BasicNode >( nodeToAttach );

        parentImpl->AddChildToModelOnly( childImpl ); 
    }
}

// *******************************
//
model::IModelNodePtr    BVSceneEditor::DetachChildNode     ( model::IModelNodePtr parent, const std::string & nodeToDetach )
{
    { parent; }
    { nodeToDetach; }

    if( parent )
    {
        // FIXME: implement
//        auto parentImpl = std::static_pointer_cast< model::BasicNode >( parent );
//        auto childImpl = std::static_pointer_cast< model::BasicNode >( nodeToDetach );

//        parentImpl->AddChildToModelOnly( childImpl ); 
    }
    return nullptr;
}

// *******************************
//
model::IModelNodePtr    BVSceneEditor::GetDetachedNode      ()
{
    return m_detachedModelNode;
}

// *******************************
//
void            BVSceneEditor::DeleteDetachedNode   ()
{
    if( m_detachedModelNode )
    {
        assert( m_detachedSceneNode );

        DeleteDetachedNodes( m_detachedModelNode, m_detachedSceneNode );
    }
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
