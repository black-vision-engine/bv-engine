#pragma once

#include "Engine/Types/Enums.h"


namespace bv {

class RenderTarget;
class Texture2D;
class Renderer;
class Camera;

class RenderTargetLogic
{
private:

    RenderTarget *  m_displayRenderTarget;
    RenderTarget *  m_auxRenderTarget;

    Texture2D *     m_readbackTexture;

    Camera *        m_displayCamera;


public:

                        RenderTargetLogic   ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~RenderTargetLogic  ();

    const Texture2D *   ReadDisplayTarget   ( Renderer * renderer );


};

} //bv
