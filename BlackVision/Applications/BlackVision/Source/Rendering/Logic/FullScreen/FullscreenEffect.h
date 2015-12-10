#pragma once

#include <string>

#include "Engine/Graphics/Shaders/RenderableEffect.h"


namespace bv {

class Renderer;
class RenderableEntity;
class Camera;

class VertexShader;
class PixelShader;


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

    static  RenderableEffectPtr CreateDefaultEffect     ( PixelShader * ps );
    static  RenderableEntity *  CreateDefaultFullscrQuad( PixelShader * ps );

    static  std::string         GetEffectShadersDir     ();
    static  std::string         ReadShaderFromFile      ( const std::string & fn );
    static  std::string         ReadFullscreenShader    ( const std::string & fn );

};

} //bv
