#pragma once

#include <string>


namespace bv {

class Renderer;
class RenderableEntity;
class Camera;
class VertexShader;


class FullscreenEffect
{
private:

    RenderableEntity *  m_fullscreenQuad;

    Camera *            m_fullscreenCamera;
    Camera *            m_lastRendererCamera;

public:

                                FullscreenEffect        ();
                                ~FullscreenEffect       ();

    virtual void                Render                  ( Renderer * renderer );

protected:

    virtual RenderableEntity *  CreateFullscreenQuad    () const = 0;

protected:

    void                        ToggleFullscreenCamera  ( Renderer * renderer );
    void                        ToggleRegularCamera     ( Renderer * renderer );

};

} //bv
