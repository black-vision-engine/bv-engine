#include "SceneEditor.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// *******************************
//
SceneEditor::SceneEditor					( Renderer * renderer, SceneNode * & rootNode )
	: m_renderer( renderer )
    , m_detachedNode( nullptr )
    , m_rootNode( rootNode )
{
}

// *******************************
//
void		SceneEditor::SetRootNode		( SceneNode * rootNode )
{
	if( m_rootNode != rootNode )
	{
		if( m_rootNode != nullptr )
		{
			DeleteRootNode();
		}
	}

	m_rootNode = rootNode;
}

// *******************************
//
bool		SceneEditor::DeleteRootNode     ()
{
	if( m_rootNode != nullptr )
	{
		DeleteNode( m_rootNode, m_renderer );
	
		m_rootNode = nullptr;

		return true;
	}

	return false;
}

// *******************************
//
void		SceneEditor::AddChildNode       ( SceneNode * parentNode, SceneNode * childNode )
{
    if( parentNode != nullptr && childNode != nullptr )
    {
        parentNode->AddChildNode( childNode );
    }
}

// *******************************
//
bool		SceneEditor::DeleteChildNode    ( SceneNode * parentNode, SceneNode * childNode )
{
    if( parentNode != nullptr && childNode != nullptr )
    {
        for( unsigned int i = 0; i < parentNode->NumChildNodes(); ++i )
        {
            if( parentNode->GetChild( i ) == childNode )
            {
                parentNode->DetachChildNode( i );

                DeleteNode( childNode, m_renderer );

                return true;
            }
        }
    }

    return false;
}

// *******************************
//
void		SceneEditor::AttachRootNode     ()
{
    if( m_rootNode != nullptr )
    {
        DeleteNode( m_rootNode, m_renderer );
    }

    m_rootNode = m_detachedNode;

    m_detachedNode = nullptr;
}

// *******************************
//
bool		SceneEditor::DetachRootNode     ()
{
    DeleteDetachedNode();
    
    if( m_rootNode != nullptr )
    {
        m_detachedNode = m_rootNode;

        m_rootNode = nullptr;

        return true;
    }

    return false;
}

// *******************************
//
bool		SceneEditor::AttachChildNode    ( SceneNode * parentNode )
{
    if( parentNode && m_detachedNode )
    {
        parentNode->AddChildNode( m_detachedNode );

        m_detachedNode = nullptr;
    
        return true;
    }

	return false;
}

// *******************************
//
bool		SceneEditor::DetachChildNode    ( SceneNode * parentNode, SceneNode * childNode )
{
    DeleteDetachedNode();

    if( parentNode && childNode )
    {
        if( parentNode->HasChild( childNode ) )
        {
            parentNode->DetachChildNode( childNode );

            m_detachedNode = childNode;

            return true;
        }
    }

	return false;
}

// *******************************
//
SceneNode *	SceneEditor::GetDetachedNode	()
{
    return m_detachedNode;
}

// *******************************
//
void		SceneEditor::DeleteDetachedNode	()
{
    if( m_detachedNode )
    {
        DeleteNode( m_detachedNode, m_renderer );
    
        m_detachedNode = nullptr;
    }
}

// *******************************
//
SceneNode *	SceneEditor::GetRootNode		()
{
	return m_rootNode;
}

// *******************************
//
void		SceneEditor::DeleteNode			( SceneNode * node, Renderer * renderer )
{
    while( node->NumChildNodes() > 0 )
    {
        DeleteNode( node->DetachChildNode( (unsigned int)0 ), renderer );
    }

    renderer->FreeAllPDResources( static_cast< RenderableEntity * >( node->GetTransformable() ) );

    delete node;
}

} //bv
