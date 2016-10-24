#pragma once

#include "Engine/Graphics/Resources/UniformBuffer.h"
#include "Engine/Graphics/SceneGraph/Camera.h"


namespace bv {

class Renderer;
class SceneNode;
class RenderableEntity;

class Scene
{
private:

    SceneNode *             m_root;

    UniformBufferUPtr       m_lightsBuffer;
    UniformBufferUPtr       m_cameraBuffer;
    CameraUPtr              m_camera;
    
    // @todo Maybe it should be moved to container class for engine grid lines.
    // In future when other helper object will apear in BV, we should implement
    // genering solution for handling it.
    RenderableEntity *      m_gridLines;
    bool                    m_gridLinesVisible;

public:

                            Scene               ();
                            ~Scene              ();

public:

    SceneNode *             GetRoot                 () const;
    void                    SetRoot                 ( SceneNode * node );

    UniformBuffer *         GetCameraBuffer         () const;
    UniformBuffer *         GetLightsBuffer         () const;
    Camera *                GetCamera               () const;

    RenderableEntity *      GetGridLines            () const;
    void                    SetGridLinesRenderable  ( RenderableEntity * renderable );
    void                    SetGridLinesVisible     ( bool visibility );
    bool                    GetGridLinesVisibility  ()  { return m_gridLinesVisible; }

};

DEFINE_PTR_TYPE( Scene );
DEFINE_UPTR_TYPE( Scene );
DEFINE_CONST_PTR_TYPE( Scene );

typedef std::vector< Scene * > SceneVec;
typedef std::vector< SceneUPtr > SceneUPtrVec;

} // bv
