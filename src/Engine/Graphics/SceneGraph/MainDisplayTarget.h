#pragma once

#include "Engine/Types/Enums.h"

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Effects/Texture2DEffect.h"


namespace bv {

class RenderTarget;
class TriangleStrip;
class Camera;
class RenderableArrayDataArraysSingleVertexBuffer;

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

    static  TriangleStrip * CreateDisplayRect           ( Texture2DPtr texture );
    static  TriangleStrip * CreateAuxRect               ( Texture2DPtr texture );

private:

    static RenderableArrayDataArraysSingleVertexBuffer *    CreateTexDispRectArrayData  ();
    static RenderableArrayDataArraysSingleVertexBuffer *    CreateTriStripArrayData     ( unsigned int numVertices, float * vbData );

    static Texture2DEffectPtr                               CreateEffectBlitTexture     ( Texture2DPtr texture );
    static Texture2DEffectPtr                               CreateEffectOverrideAlpha   ( Texture2DPtr texture );

};

} //bv
