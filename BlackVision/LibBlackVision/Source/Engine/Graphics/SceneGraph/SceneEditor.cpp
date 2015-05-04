#include "SceneEditor.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Renderers/Renderer.h"


namespace bv {

// *******************************
//
SceneEditor::SceneEditor					( Renderer * renderer, SceneNode ** rootNode )
	: m_renderer( renderer )
	, m_rootNode( rootNode )
{
}

// *******************************
//
void		SceneEditor::SetRootNode		( SceneNode * rootNode )
{
	if( *m_rootNode != rootNode )
	{
		if( *m_rootNode != nullptr )
		{
			DeleteRootNode();
		}
	}

	*m_rootNode = rootNode;
}

// *******************************
//
bool		SceneEditor::DeleteRootNode     ()
{
	if( m_rootNode != nullptr )
	{
		DeleteNode( *m_rootNode, m_renderer );
	
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
    { parentNode; childNode; }

	return false;
}

// *******************************
//
void		SceneEditor::AttachRootNode     ()
{
	// FIXME: implement
}

// *******************************
//
bool		SceneEditor::DetachRootNode     ()
{
	return false;
}

// *******************************
//
bool		SceneEditor::AttachChildNode    ( SceneNode * parentNode )
{
	// FIXME: implement
	{ parentNode; }

	return false;
}

// *******************************
//
bool		SceneEditor::DetachChildNode    ( SceneNode * parentNode, SceneNode * childNode )
{
	// FIXME: implement
	{ parentNode; childNode; }

	return false;
}

// *******************************
//
SceneNode *	SceneEditor::GetDetachedNode	()
{
	// FIXME: implement
	return nullptr;
}

// *******************************
//
void		SceneEditor::DeleteDetachedNode	()
{
	// FIXME: implement
}

// *******************************
//
SceneNode *	SceneEditor::GetRootNode		()
{
	return *m_rootNode;
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
