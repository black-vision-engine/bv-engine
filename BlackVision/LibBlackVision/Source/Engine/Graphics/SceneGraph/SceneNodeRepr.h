#pragma once

#include <vector>

#include "Engine/Graphics/SceneGraph/TransformableEntity.h"

#include "CoreDEF.h"


namespace bv {

class SceneNode;
class RenderableEntity;

class SceneNodeRepr
{
private:

    typedef std::vector< SceneNode *>            SceneNodeVec;

private:

    SceneNodeVec            m_sceneNodes;

    TransformableEntity *   m_transformable;

public:

                            SceneNodeRepr       ( TransformableEntity * transformable );
                            ~SceneNodeRepr      ();

    SizeType                NumChildNodes       () const;

    void                    AddChildNode        ( SceneNode * child );
    void                    DetachChildNode     ( SceneNode * node );
    SceneNode *             DetachChildNode     ( unsigned int idx );

    SceneNode *             GetChild            ( unsigned int idx );
    bool                    HasChild            ( SceneNode * node ) const;

    TransformableEntity *   GetTransformable    ();

//FIXME: acces from BVSceneTools
public:

    void                    SetTransformable    ( TransformableEntity * transformable );
    void                    DeleteTransformable ();

public:

    void                    Update              ( const Transform & parentTransform );

    // FIXME: think of some better approach to dynamic node state manipulation
    friend class BVSceneTools;
};

RenderableEntity * renderable( SceneNodeRepr * nodeRepr );

} // bv
