#pragma once

#include <vector>

#include "Engine/Graphics/SceneGraph/TransformableEntity.h"
#include "Engine/Audio/AudioEntity.h"
#include "Engine/Graphics/Effects/NodeEffect/NodeEffect.h"
#include "Mathematics/Box.h"

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

    SceneNodeVec                    m_sceneNodes;

    NodeEffectPtr                   m_nodeEffect;

    TransformableEntity *           m_transformable;

    bool                            m_visible;

    bool                            m_drawBoundingBox;
    glm::vec4                       m_boundingBoxColor;

    const mathematics::Box *        m_boundingBox;

    audio::AudioEntity *            m_audio;

    SceneNodePerformance *          m_performanceData;

public:

                            SceneNode           ( TransformableEntity * transformable = nullptr );
                            ~SceneNode          ();

    SizeType                NumChildNodes       () const;

    void                    AddChildNode        ( SceneNode * child );
    void                    AddChildNode        ( SceneNode * child, UInt32 idx );
    void                    DetachChildNode     ( SceneNode * node );
    SceneNode *             DetachChildNode     ( unsigned int idx );

    SceneNode *             GetChild            ( unsigned int idx );
    bool                    HasChild            ( SceneNode * node ) const;

    TransformableEntity *   GetTransformable    ();

    audio::AudioEntity *    GetAudio            ();
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

    void                    DeleteAudio         ();

public:

    void                    Update              ( const Transform & parentTransform );

    bool                    IsVisible           () const;
    void                    SetVisible          ( bool visible );

    // FIXME: think of some better approach to dynamic node state manipulation
    friend class BVProjectTools;
};

} // bv
