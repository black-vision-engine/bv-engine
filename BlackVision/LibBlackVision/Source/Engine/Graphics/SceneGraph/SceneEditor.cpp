#include "SceneEditor.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"


namespace bv {

// *******************************
//
SceneEditor::SceneEditor					()
{
}

// *******************************
//
void		SceneEditor::SetRootNode		( SceneNode * rootNode )
{
	// FIXME: implement
	{ rootNode; }
}

// *******************************
//
bool		SceneEditor::DeleteRootNode     ( SceneNode * rootNode )
{
	// FIXME: implement
	{ rootNode; }
	return false;
}

// *******************************
//
void		SceneEditor::AddChildNode       ( SceneNode * parentNode, SceneNode * childNode )
{
	// FIXME: implement
	{ parentNode; childNode; }
}

// *******************************
//
bool		SceneEditor::DeleteChildNode    ( SceneNode * parentNode, SceneNode * childNode )
{
	// FIXME: implement
	{ parentNode; childNode; }

	return false;
}

// *******************************
//
void		SceneEditor::AttachRootNode     ( SceneNode * rootNode )
{
	// FIXME: implement
	{ rootNode; }
}

// *******************************
//
bool		SceneEditor::DetachRootNode     ( SceneNode * rootNode )
{
	// FIXME: implement
	{ rootNode; }

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

} //bv
