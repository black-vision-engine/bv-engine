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
void            BVSceneEditor::SetRootNode          ( model::IModelNodePtr rootNode )
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
bool            BVSceneEditor::DeleteRootNode       ()
{
    auto modelRoot  = std::static_pointer_cast< model::BasicNode >( S()->m_pModelSceneRoot );
    auto engineRoot = S()->m_pEngineSceneRoot;

    if( modelRoot )
    {
        assert( engineRoot );

        UnregisterUpdaters( modelRoot );
        RemoveNodeMappings( modelRoot );

        S()->m_pModelSceneRoot  = nullptr;
        S()->m_pEngineSceneRoot = nullptr;

        SceneNode::DeleteNode( engineRoot, S()-> m_renderer );
    }

    return modelRoot != nullptr;
}

/*
    // Dynamic tree manipulation API
    void                    SetRootNode         ( model::IModelNodePtr rootNode );
    bool                    DeleteRootNode      ();

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

} //bv
