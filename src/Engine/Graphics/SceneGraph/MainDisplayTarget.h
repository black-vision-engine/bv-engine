#pragma once

#include "Engine/Types/Enums.h"


namespace bv {

class RenderTarget;
class TriangleStrip;
class Camera;

class MainDisplayTarget
{
private:

        MainDisplayTarget   ();
        ~MainDisplayTarget  ();

public:

    static  RenderTarget *  CreateDisplayRenderTarget   ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
    static  RenderTarget *  CreateAuxRenderTarget       ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );

};

} //bv
