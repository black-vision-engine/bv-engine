#pragma once

#include "Engine/Types/Enums.h"


namespace bv {

class RenderTarget;
class Texture2D;
class Renderer;
class Camera;
class TriangleStrip;
class Rednerer;


class OffscreenRenderLogic
{
private:

    RenderTarget *  m_displayRenderTarget;
    RenderTarget *  m_auxRenderTarget;
    
    TriangleStrip * m_displayQuad;
    TriangleStrip * m_auxQuad;

    Texture2D *     m_readbackTexture;

    Camera *        m_displayCamera;
    Camera *        m_rendererCamera;

    bool            m_displayRTEnabled;
    bool            m_auxRTEnabled;

public:

                        OffscreenRenderLogic        ( unsigned int width, unsigned int height, Camera * camera = nullptr, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~OffscreenRenderLogic       ();

    void                SetRendererCamera           ( Camera * camera );

    void                EnableDisplayRenderTarget   ( Renderer * renderer );
    void                EnableAuxRenderTarget       ( Renderer * renderer );

    void                DisableDisplayRenderTarget  ( Renderer * renderer );
    void                DisableAuxRenderTarget      ( Renderer * renderer );

    void                DrawDisplayRenderTarget     ( Renderer * renderer );

    const Texture2D *   ReadDisplayTarget           ( Renderer * renderer );


};

} //bv
