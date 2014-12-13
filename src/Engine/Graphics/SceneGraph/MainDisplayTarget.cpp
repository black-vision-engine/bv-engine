#include "MainDisplayTarget.h"

#include <cassert>
#include <vector>

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Resources/RenderTarget.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"

#include "Engine/Graphics/Effects/Texture2DEffect.h"


namespace bv {

// **************************
//
RenderTarget *  MainDisplayTarget::CreateDisplayRenderTarget    ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, true, false, RenderTarget::RTSemantic::S_DRAW_READ );
}

// **************************
//
RenderTarget *  MainDisplayTarget::CreateAuxRenderTarget        ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, true, false, RenderTarget::RTSemantic::S_DRAW_ONLY );
}

// **************************
//
Camera *        MainDisplayTarget::CreateDisplayCamera          ()
{
    auto camera = new Camera( false );

    //FIXME: upewnic sie, ze tu sa prawdziwe macierze jednostkowe bez zadnych piardow z zaokraglen (i jesli nie sa, to recznie to zapewnic ustawiajac macierze z palca)
    //Ponizszy kod tworzy poprawne macierze identycznosciowe, ale konstruktor domyslnie tez to robi - kod zostaje dla celow szkoleniowych, ale zostawiamy wyniki z konstruktora macierzy w kamerze
    
    //camera->SetFrame( glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 0.f, -1.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //camera->SetFrustum( -1.f, 1.f, -1.f, 1.f, 1.f, -1.f );

    //FIXME: pewnie mozna na spokojnie te macierze potem poprawic na identycznosciowe (skoro kod do ich generacji jest poporawny i wiadomo, na jakiej zasadzie dziala)
    return camera;
}

// **************************
//
TriangleStrip * MainDisplayTarget::CreateDisplayRect            ( Texture2DPtr texture )
{
    auto rad = CreateTexDispRectArrayData();
    auto effect = CreateEffectBlitTexture( texture );

    return new TriangleStrip( rad, effect );
}

// **************************
//
TriangleStrip * MainDisplayTarget::CreateAuxRect               ( Texture2DPtr texture )
{
    auto rad = CreateTexDispRectArrayData();
    auto effect = CreateEffectOverrideAlpha( texture );

    return new TriangleStrip( rad, effect );    
}

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer * MainDisplayTarget::CreateTexDispRectArrayData  ()
{
    float z = 0.0f;
    float d = 1.0f;

    float vbData[] = { -d, -d, z, 0.f, 0.f,   //V0, U0
                        d, -d, z, 1.f, 0.f,   //V1, U1
                       -d,  d, z, 0.f, 1.f,   //V2, U2
                        d,  d, z, 1.f, 1.f }; //V3, U3

    return CreateTriStripArrayData( 4, vbData );
}

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer * MainDisplayTarget::CreateTriStripArrayData( unsigned int numVertices, float * vbData )
{
    unsigned int vertexSize = 4 * sizeof( float ) + 2 * sizeof( float );

    VertexBuffer * vb       = new VertexBuffer( 4, vertexSize, DataBuffer::Semantic::S_STATIC );
    VertexDescriptor * vd   = VertexDescriptor::Create( 2,
                                                        0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION,
                                                        1, AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD );

    memcpy( vb->Data(), vbData, numVertices * vertexSize );

    VertexArraySingleVertexBuffer * vao = new VertexArraySingleVertexBuffer( vb, vd );
    vao->AddCCEntry( 4 );

    RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );

    return rad;
}

// **************************
//
Texture2DEffectPtr   MainDisplayTarget::CreateEffectBlitTexture     ( Texture2DPtr texture )
{
    return std::make_shared<Texture2DEffect>( texture );
}

// **************************
//
Texture2DEffectPtr   MainDisplayTarget::CreateEffectOverrideAlpha   ( Texture2DPtr texture )
{
    return std::make_shared<Texture2DEffect>( texture, true );
}

} //bv
