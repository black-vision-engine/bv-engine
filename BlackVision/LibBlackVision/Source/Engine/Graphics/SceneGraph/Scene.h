#pragma once


namespace bv {

class Renderer;
class SceneNode;
class UniformBuffer;
class RenderableEntity;
class Camera;

class Scene
{
private:

    SceneNode *             m_root;

    UniformBuffer *         m_lightsBuffer;
    UniformBuffer *         m_cameraBuffer;
    Camera *                m_camera;
    
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
DEFINE_CONST_PTR_TYPE( Scene );

typedef std::vector< Scene * > SceneVec;

} // bv
