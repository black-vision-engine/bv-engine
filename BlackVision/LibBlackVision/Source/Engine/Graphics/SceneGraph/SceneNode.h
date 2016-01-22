#pragma once

#include <vector>

#include "Engine/Graphics/SceneGraph/TransformableEntity.h"
#include "Engine/Graphics/Effects/NodeEffects/NodeEffect.h"
#include "Engine/Graphics/Effects/NodeEffectTr/NodeEffectTr.h"

#include "CoreDEF.h"


namespace bv {

class IValue;
class TransformableEntity;
class Renderer;


class SceneNode
{
private:

    typedef std::vector<SceneNode *>            SceneNodeVec;

private:

    SceneNodeVec            m_sceneNodes;
    NodeEffectPtr           m_nodeEffect;

    NodeEffectTrPtr         m_nodeEffectTr;

    TransformableEntity *   m_transformable;

    bool                    m_visible;

public:

                            SceneNode           ( TransformableEntity * transformable = nullptr );
                            ~SceneNode          ();

    SizeType                NumChildNodes       () const;

    void                    AddChildNode        ( SceneNode * child );
    void                    DetachChildNode     ( SceneNode * node );
    SceneNode *             DetachChildNode     ( unsigned int idx );

    SceneNode *             GetChild            ( unsigned int idx );
    bool                    HasChild            ( SceneNode * node ) const;

    TransformableEntity *   GetTransformable    ();
   
    NodeEffectPtr           GetNodeEffect       ();
    void                    SetNodeEffect       ( NodeEffectPtr nodeEffect );

    NodeEffectTrPtr         GetNodeEffectTr     ();
    void                    SetNodeEffectTr     ( NodeEffectTrPtr nodeEffect );

private:

    void                    SetTransformable    ( TransformableEntity * transformable );
    void                    DeleteTransformable ();

public:

    void                    Update              ( const std::vector< Transform > & parentTransforms );

    bool                    IsVisible           () const;
    void                    SetVisible          ( bool visible );

    // FIXME: think of some better approach to dynamic node state manipulation
    friend class BVSceneTools;
};

} // bv
