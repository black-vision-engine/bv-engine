#pragma once


namespace bv {

class SceneNode;
class Renderer;

class SceneEditor
{
private:

	SceneNode **	m_rootNode;
	SceneNode *		m_detachedNode;
	Renderer *		m_renderer;

public:

							SceneEditor			( Renderer * renderer, SceneNode ** rootNode );

    void                    SetRootNode         ( SceneNode * rootNode );
    bool                    DeleteRootNode      ();

    void                    AddChildNode        ( SceneNode * parentNode, SceneNode * childNode );
    bool                    DeleteChildNode     ( SceneNode * parentNode, SceneNode * childNode );

    void                    AttachRootNode      ();
    bool                    DetachRootNode      ();

	bool                    AttachChildNode     ( SceneNode * parentNode );
    bool                    DetachChildNode     ( SceneNode * parentNode, SceneNode * childNode );

    SceneNode *				GetDetachedNode     ();
    void                    DeleteDetachedNode  ();

	SceneNode *				GetRootNode			();

private:

	void					DeleteNode			( SceneNode * node, Renderer * renderer );

};

} //bv
