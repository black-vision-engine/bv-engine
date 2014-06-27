#pragma once

#include "Engine/Types/Enums.h"


namespace bv {

class RenderTarget;
class TriangleStrip;
class Camera;
class RenderableArrayDataArraysSingleVertexBuffer;
class Texture2DEffect;
class Texture2D;

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

    static  TriangleStrip * CreateDisplayRect           ( Texture2D * texture );
    static  TriangleStrip * CreateAuxRect               ( Texture2D * texture );

private:

    static RenderableArrayDataArraysSingleVertexBuffer *    CreateTexDispRectArrayData  ();
    static RenderableArrayDataArraysSingleVertexBuffer *    CreateTriStripArrayData     ( unsigned int numVertices, float * vbData );

    static Texture2DEffect *                                CreateEffectBlitTexture     ( Texture2D * texture );
    static Texture2DEffect *                                CreateEffectOverrideAlpha   ( Texture2D * texture );

};

} //bv
