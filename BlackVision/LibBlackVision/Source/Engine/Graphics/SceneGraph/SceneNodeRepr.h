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

    SceneNode *                 m_owner;

    SceneNodeVec                m_sceneNodes;

    TransformableEntity *       m_transformable;
    audio::AudioEntity *        m_audio;

    const mathematics::Box *    m_boundingBox;

    SceneNodePerformance *      m_performanceData;

public:

                            SceneNodeRepr       ( TransformableEntity * transformable, SceneNode * owner );
                            ~SceneNodeRepr      ();

    SizeType                NumChildNodes       () const;

    void                    AddChildNode        ( SceneNode * child );
    void                    AddChildNode        ( SceneNode * child, UInt32 idx );
    void                    DetachChildNode     ( SceneNode * node );
    SceneNode *             DetachChildNode     ( unsigned int idx );

    SceneNode *             GetChild            ( unsigned int idx );
    bool                    HasChild            ( SceneNode * node ) const;

    TransformableEntity *   GetTransformable    ();
    
    audio::AudioEntity *    GetAudio            () const;
    void                    SetAudio            ( audio::AudioEntity * audio );

    void                    SetBoundingBox      ( const math::Box * bb );
    const math::Box *       GetBoundingBox      () const;

    SceneNodePerformance *  GetPerformanceData  ();

//FIXME: hopefully temporary hack for RenderingQueue
    SceneNode *             GetOwnerNode        ()  { return m_owner; }

//FIXME: access from BVSceneTools
public:

    void                    SetTransformable    ( TransformableEntity * transformable );
    void                    DeleteTransformable ();

    void                    DeleteAudio         ();

public:

    void                    Update              ( const Transform & parentTransform );

    // FIXME: think of some better approach to dynamic node state manipulation
    friend class BVSceneTools;
};

RenderableEntity * renderable( SceneNodeRepr * nodeRepr );

} // bv
