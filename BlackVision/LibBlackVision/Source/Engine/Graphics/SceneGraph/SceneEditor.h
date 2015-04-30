#pragma once


namespace bv {

class SceneNode;

class SceneEditor
{
private:

	SceneNode *		m_detachedNode;

public:

							SceneEditor			();

    void                    SetRootNode         ( SceneNode * rootNode );
    bool                    DeleteRootNode      ( SceneNode * rootNode );

    void                    AddChildNode        ( SceneNode * parentNode, SceneNode * childNode );
    bool                    DeleteChildNode     ( SceneNode * parentNode, SceneNode * childNode );

    void                    AttachRootNode      ( SceneNode * rootNode );
    bool                    DetachRootNode      ( SceneNode * rootNode );

	bool                    AttachChildNode     ( SceneNode * parentNode );
    bool                    DetachChildNode     ( SceneNode * parentNode, SceneNode * childNode );

    SceneNode *				GetDetachedNode     ();
    void                    DeleteDetachedNode  ();

};

} //bv
