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

#include "Engine/Graphics/Effects/DefaultEffect.h"


namespace bv {

// **************************
//
RenderTarget *  MainDisplayTarget::CreateDisplayRenderTarget    ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, false, false );
}

// **************************
//
RenderTarget *  MainDisplayTarget::CreateAuxRenderTarget        ( unsigned int width, unsigned int height, TextureFormat fmt )
{
    assert( width > 0 );
    assert( height > 0 );

    std::vector< TextureFormat > fmts( 1 );
    fmts[ 0 ] = fmt;

    return new RenderTarget( fmts, width, height, true, false );
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
TriangleStrip * MainDisplayTarget::CreateDisplayRect            ()
{
    auto rad = CreateTexDispRectArrayData();
    auto effect = CreateEffectBlitTexture();

    return new TriangleStrip( rad, effect );
}

// **************************
//
TriangleStrip * MainDisplayTarget::CreateAuxRect               ()
{
    auto rad = CreateTexDispRectArrayData();
    auto effect = CreateEffectOverrideAlpha();

    return new TriangleStrip( rad, effect );    
}

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer * MainDisplayTarget::CreateTexDispRectArrayData  ()
{
    float vbData[] = { -1.f, -1.f, 0.f, 0.f, 0.f,   //V0, U0
                        1.f, -1.f, 0.f, 1.f, 0.f,   //V1, U1
                       -1.f,  1.f, 0.f, 0.f, 1.f,   //V2, U2
                        1.f,  1.f, 0.f, 1.f, 1.f }; //V3, U3

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
    RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );

    return rad;
}

// **************************
//
RenderableEffect *                            MainDisplayTarget::CreateEffectBlitTexture     ()
{
    //FIXME: implement
    return new DefaultEffect( nullptr, nullptr, nullptr, nullptr );
}

// **************************
//
RenderableEffect *                            MainDisplayTarget::CreateEffectOverrideAlpha   ()
{
    //FIXME: implement
    return new DefaultEffect( nullptr, nullptr, nullptr, nullptr );
}

} //bv
