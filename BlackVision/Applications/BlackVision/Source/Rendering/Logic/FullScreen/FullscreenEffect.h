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

    virtual RenderableEntity *  CreateFullscreenQuad    ( Renderer * renderer ) const = 0; //FIXME: make sure whether the renderer is really necessary

protected:

    void                        ToggleFullscreenCamera  ( Renderer * renderer );
    void                        ToggleRegularCamera     ( Renderer * renderer );

    virtual VertexShader *      CreateVS                ( unsigned int numUVChannels ) const;

private:

    std::string                 GetVSShaderSource       ( unsigned int numUVChannels ) const;

};

} //bv
