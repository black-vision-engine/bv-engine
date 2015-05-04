#include "BVSceneEditor.h"

#include "Engine/Models/BVScene.h"
#include "Engine/Models/Updaters/UpdatersManager.h"
#include "Engine/Models/BVSceneTools.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Models/ModelSceneEditor.h"
#include "Engine/Graphics/SceneGraph/SceneEditor.h"


namespace bv {
    
namespace {
// *******************************
//
model::BasicNodePtr QueryTyped( model::IModelNodePtr node )
{
    return std::static_pointer_cast< model::BasicNode >( node );
}

} // anonymous

// *******************************
//
BVSceneEditor::BVSceneEditor                ( BVScene * scene )
{
    assert( scene != nullptr );
    assert( scene->m_renderer != nullptr );

    m_modelSceneEditor = new model::ModelSceneEditor( scene->m_pModelSceneRoot );
    m_engineSceneEditor = new SceneEditor( scene->m_renderer, scene->m_pEngineSceneRoot );
}

// *******************************
//
void    BVSceneEditor::SetRootNode          ( model::IModelNodePtr rootNode )
{
    auto root = QueryTyped( rootNode );

    if( m_modelSceneEditor->GetRootNode() != root )
    {
        MappingsCleanup( m_modelSceneEditor->GetRootNode() );

        m_modelSceneEditor->SetRootNode( root );
        m_engineSceneEditor->SetRootNode( BVSceneTools::BuildEngineSceneNode( root, m_nodesMapping ) );
    }
}

// *******************************
//
bool    BVSceneEditor::DeleteRootNode       ()
{
    auto root = m_modelSceneEditor->GetRootNode();

    if( root )
    {
        MappingsCleanup( root );

        m_modelSceneEditor->DeleteRootNode();
        m_engineSceneEditor->DeleteRootNode();
    
        return true;
    }

    return false;
}

// *******************************
//
void    BVSceneEditor::AddChildNode         ( model::IModelNodePtr parentNode, model::IModelNodePtr childNode )
{
    if( parentNode &&  childNode )
    {
        auto engineParent = GetEngineNode( parentNode );
        auto engineChild = BVSceneTools::BuildEngineSceneNode( QueryTyped( childNode ), m_nodesMapping );

        m_modelSceneEditor->AddChildNode( QueryTyped( parentNode ), QueryTyped( childNode ) );
        m_engineSceneEditor->AddChildNode( engineParent, engineChild );
    }
}

// *******************************
//
bool    BVSceneEditor::DeleteChildNode      ( model::IModelNodePtr parentNode, const std::string & childNodeName )
{    
    if( parentNode )
    {
        auto modelParentNode = QueryTyped( parentNode );
        auto modelChildNode = QueryTyped( modelParentNode->GetChild( childNodeName ) );

        if( modelChildNode )
        {
            m_modelSceneEditor->DeleteChildNode( modelParentNode, childNodeName );
            m_engineSceneEditor->DeleteChildNode( GetEngineNode( parentNode ), GetEngineNode( modelChildNode ) );

            MappingsCleanup( modelChildNode );

            return true;
        }
    }

    return false;
}

// *******************************
//
void    BVSceneEditor::AttachRootNode      ()
{
    MappingsCleanup( m_modelSceneEditor->GetRootNode() );

    m_modelSceneEditor->AttachRootNode();
    m_engineSceneEditor->AttachRootNode();
}

// *******************************
//
bool    BVSceneEditor::DetachRootNode      ()
{
    MappingsCleanup( m_modelSceneEditor->GetDetachedNode() );

    auto detachModel    = m_modelSceneEditor->DetachRootNode();
    auto detachEngine   = m_engineSceneEditor->DetachRootNode();

    return detachModel && detachEngine;
}

// *******************************
//
bool                    BVSceneEditor::AttachChildNode     ( model::IModelNodePtr parent )
{
    if( parent )
    {
        auto attachModel    = m_modelSceneEditor->AttachChildNode( QueryTyped( parent ) );
        auto attachEngine   = m_engineSceneEditor->AttachChildNode( GetEngineNode( parent ) );

        return attachModel && attachEngine;
    }

    return false;
}

// *******************************
//
bool                    BVSceneEditor::DetachChildNode     ( model::IModelNodePtr parent, const std::string & nodeToDetach )
{
    MappingsCleanup( m_modelSceneEditor->GetDetachedNode() );

    if( parent )
    {
        auto childNode = parent->GetChild( nodeToDetach );

        if( childNode )
        {
            auto detachModel    = m_modelSceneEditor->DetachChildNode( QueryTyped( parent ), nodeToDetach );
            auto detachEngine   = m_engineSceneEditor->DetachChildNode( GetEngineNode( parent ), GetEngineNode( childNode ) );
        
            return detachModel && detachEngine;
        }
    }

    return false;
}

// *******************************
//
void            BVSceneEditor::DeleteDetachedNodes          ()
{
    MappingsCleanup( m_modelSceneEditor->GetDetachedNode() );

    m_modelSceneEditor->DeleteDetachedNode();
    m_engineSceneEditor->DeleteDetachedNode();
}

// *******************************
//
model::IModelNodePtr    BVSceneEditor::GetRootNode          ()
{
    return m_modelSceneEditor->GetRootNode();
}

// *******************************
//
void                    BVSceneEditor::MappingsCleanup      ( model::IModelNodePtr node )
{
    if( node )
    {
        RemoveNodeMapping( node );
        UnregisterUpdaters( node );
    }
}

// *******************************
//
void                    BVSceneEditor::RemoveNodeMapping    ( model::IModelNodePtr node )
{
    assert( m_nodesMapping.find( node.get() ) != m_nodesMapping.end() );
    m_nodesMapping.erase( node.get() );
    
    auto modelNode = QueryTyped( node );

    for( unsigned int i = 0; i < node->GetNumChildren(); ++i )
    {
        auto child = modelNode->GetChild( i );

        RemoveNodeMapping( child );
    }
}

// *******************************
//
void                    BVSceneEditor::UnregisterUpdaters   ( model::IModelNodePtr node )
{
    UpdatersManager::Get().RemoveNodeUpdater( node.get() );

    for( unsigned int i = 0; i < node->GetNumChildren(); ++i )
    {
        auto child = QueryTyped( node )->GetChild( i );

        UnregisterUpdaters( child );
    }
}

// *******************************
//
SceneNode *             BVSceneEditor::GetEngineNode        ( model::IModelNodePtr node )
{
    return m_nodesMapping[ node.get() ];
}

} //bv
