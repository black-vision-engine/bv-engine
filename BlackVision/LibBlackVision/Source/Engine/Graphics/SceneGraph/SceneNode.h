#pragma once

#include <vector>

#include "Engine/Graphics/SceneGraph/TransformableEntity.h"
#include "Engine/Audio/AudioEntity.h"
#include "Engine/Graphics/Effects/NodeEffect/NodeEffect.h"
#include "Mathematics/Box.h"

#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NodeEffect/NNodeEffect.h"

#include "SceneNodePerformance.h"

#include "CoreDEF.h"


namespace bv {

class IValue;
class TransformableEntity;
class Renderer;

namespace math = mathematics;

class SceneNode
{
private:

    typedef std::vector<SceneNode *>            SceneNodeVec;

private:

    SceneNodeRepr *     m_repr;

    NodeEffectPtr       m_nodeEffect;
    nrl::NNodeEffectPtr m_nNodeEffect;

    //FIXME: use some sort of caps/flag here (instead of a single flag)
    bool                m_visible;
    bool                m_drawBoundingBox;
    glm::vec4           m_boundingBoxColor;

public:

                            SceneNode           ( TransformableEntity * transformable = nullptr );
                            ~SceneNode          ();

    SceneNodeRepr *         GetRepr             ();

    //FIXME: remove
    SizeType                NumChildNodes       () const;

    void                    AddChildNode        ( SceneNode * child );
    void                    AddChildNode        ( SceneNode * child, UInt32 idx );
    void                    DetachChildNode     ( SceneNode * node );
    SceneNode *             DetachChildNode     ( unsigned int idx );

    SceneNode *             GetChild            ( unsigned int idx );
    bool                    HasChild            ( SceneNode * node ) const;

    //FIXME: remove
    TransformableEntity *   GetTransformable    ();
    audio::AudioEntity *    GetAudio            () const;
    void                    SetAudio            ( audio::AudioEntity * audio );
   
    NodeEffectPtr           GetNodeEffect       ();
    void                    SetNodeEffect       ( NodeEffectPtr nodeEffect );

    void                    SetBoundingBox      ( const math::Box * bb );
    const math::Box *       GetBoundingBox      () const;
    bool                    IsSelected          () const;
    glm::vec4               GetBoundingBoxColor () const;
    void                    Select              ( glm::vec4 color );
    void                    Unselect            ();

    SceneNodePerformance *  GetPerformanceData  ();

private:

    void                    SetTransformable    ( TransformableEntity * transformable );
    void                    DeleteTransformable ();

public:

    void                    Update              ( const Transform & parentTransform );

    bool                    IsVisible           () const;
    void                    SetVisible          ( bool visible );

    // FIXME: think of some better approach to dynamic node state manipulation
    friend class BVProjectTools;
};

RenderableEntity *  renderable  ( SceneNode * node );

} // bv
