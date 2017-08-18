#pragma once

#include <vector>

#include "CoreDEF.h"

#include "Engine/Graphics/SceneGraph/TransformableEntity.h"
#include "Engine/Graphics/SceneGraph/TransformableEntity.h"

#include "Engine/Audio/AudioEntity.h"
#include "Mathematics/Box.h"

#include "EngineGizmoContainer.h"

#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeEffect/NodeEffect.h"

#include "SceneNodePerformance.h"


namespace bv {

class IValue;
class TransformableEntity;
class SceneNodeRepr;
class RenderableEntity;

namespace math = mathematics;

class SceneNode
{
private:

    SceneNodeRepr *     m_repr;

    NodeEffectPtr       m_nodeEffect;

    //FIXME: use some sort of caps/flag here (instead of a single flag)
    bool                m_visible;
    bool                m_nodeEffectEnabled;

    EngineGizmoContainerUPtr        m_gizmos;

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
   
    //FIXME: move to some wrapper class holding nnodeeffect along with node related state (right now only enabled/disabled)
    bool                    IsNodeEffectEnabled () const;
    void                    EnableNodeEffect    ();
    void                    DisableNodeEffect   ();

    // FIXME: nrl - rename to GetNodeEffect during nrl -> default migration
    NodeEffectPtr           GetNodeEffect       ();
    void                    SetNodeEffect       ( NodeEffectPtr nodeEffect );

    void                    SetBoundingBox      ( const math::Box * bb );
    const math::Box *       GetBoundingBox      () const;
    bool                    IsSelected          () const;
    void                    Select              ( glm::vec4 color );
    void                    Unselect            ();

    SceneNodePerformance *  GetPerformanceData  ();


public:

    void                    AddGizmo            ( SceneNode * gizmoRoot, UInt32 idx = std::numeric_limits< UInt32 >::max() );
    void                    RemoveGizmo         ( UInt32 idx );
    void                    RemoveGizmo         ( SceneNode * gizmoRoot );

    SceneNode *             GetGizmo            ( UInt32 idx ) const;
    UInt32                  GetNumGizmos        () const;

private:

    EngineGizmoContainer *  AllocateGizmos      ();
    void                    DeallocateGizmos    ();

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
