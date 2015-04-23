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
bool    BVSceneEditor::DeleteChildNode     ( model::IModelNodePtr parentNode, const std::string & childNodeName )
{
    auto parentModelNode  = std::static_pointer_cast< model::BasicNode >( parentNode );
    
    if( parentModelNode )
    {
        auto modelChildNode = std::static_pointer_cast< model::BasicNode >( parentModelNode->GetChild( childNodeName ) );

        if( modelChildNode )
        {
            auto engineChildNode = m_nodesMapping[ modelChildNode.get() ];
            assert( engineChildNode != nullptr );

            DetachChildNode( parentModelNode, modelChildNode );

            DeleteDetachedNodes( modelChildNode, engineChildNode );

            return true;
        }
    }

    return false;
}

/*

    void                    AddChildNode        ( model::IModelNodePtr parentNode, model::IModelNodePtr childNode );
    bool                    DeleteChildNode     ( model::IModelNodePtr parentNode, const std::string & childNodeName );

    void                    AttachChildNode     ( model::IModelNodePtr parent, model::IModelNodePtr nodeToAttach );
    model::IModelNodePtr    DetachNodeFrom      ( model::IModelNodePtr parent, const std::string & nodeToDetach );

    void                    AttachRootNode      ( model::IModelNodePtr rootNode );
    model::IModelNodePtr    DetachRootNode      ();

    RenameNode                                  ( model::IModelNodePtr node, const std::string & newName );
*/

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
    auto engineParent = m_nodesMapping[ parentNode.get() ];
    auto engineChild  = m_nodesMapping[ childNode.get() ];

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
