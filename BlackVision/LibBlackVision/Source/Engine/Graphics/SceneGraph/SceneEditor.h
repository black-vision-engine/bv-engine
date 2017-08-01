#pragma once

#include "CoreDEF.h"

namespace bv {

class SceneNode;
class Renderer;

class SceneEditor
{
private:

	SceneNode * &	m_rootNode;
	SceneNode *		m_detachedNode;
	Renderer *		m_renderer;

private:

                            SceneEditor         ( const SceneEditor & other );
    SceneEditor &           operator =          ( const SceneEditor & other );

public:

							SceneEditor			( Renderer * renderer, SceneNode * & rootNode );

    void                    AddChildNode        ( SceneNode * parentNode, SceneNode * childNode );
    void                    AddChildNode        ( SceneNode * parentNode, SceneNode * childNode, UInt32 idx );
    bool                    DeleteChildNode     ( SceneNode * parentNode, SceneNode * childNode );

    void                    AddGizmoNode        ( SceneNode * parentNode, SceneNode * childNode );
    bool                    DeleteGizmoNode     ( SceneNode * parentNode, SceneNode * childNode );

	bool                    AttachChildNode     ( SceneNode * parentNode );
	bool                    AttachChildNode     ( SceneNode * parentNode, UInt32 idx );
    bool                    DetachChildNode     ( SceneNode * parentNode, SceneNode * childNode );

    SceneNode *				GetDetachedNode     ();
    void                    DeleteDetachedNode  ();

	SceneNode *				GetRootNode			();

private:

	void					DeleteNode			( SceneNode * node, Renderer * renderer );

};

} //bv
