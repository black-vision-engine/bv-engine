#pragma once

#include "Engine/Types/Enums.h"


namespace bv {

class RenderTarget;
class Texture2D;
class Renderer;
class Camera;
class TriangleStrip;

class OffscreenRenderLogic
{
private:

    RenderTarget *  m_displayRenderTarget;
    RenderTarget *  m_auxRenderTarget;
    
    TriangleStrip * m_displayQuad;
    TriangleStrip * m_auxQuad;

    Texture2D *     m_readbackTexture;

    Camera *        m_displayCamera;


public:

                        OffscreenRenderLogic    ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~OffscreenRenderLogic   ();

    const Texture2D *   ReadDisplayTarget       ( Renderer * renderer );


};

} //bv
