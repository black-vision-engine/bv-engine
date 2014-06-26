#pragma once

#include "Engine/Types/Enums.h"


namespace bv {

class RenderTarget;
class TriangleStrip;
class Camera;

// FIXME: implement additional sized rectangles (useful for rendering geometry that does not require the whole viewport - scissors like)
class MainDisplayTarget
{
private:

        MainDisplayTarget   ();
        ~MainDisplayTarget  ();

public:

    static  RenderTarget *  CreateDisplayRenderTarget   ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
    static  RenderTarget *  CreateAuxRenderTarget       ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );

    static  Camera *        CreateDisplayCamera         ();

    static  TriangleStrip * CreateDisplayRect           ();

};

} //bv
