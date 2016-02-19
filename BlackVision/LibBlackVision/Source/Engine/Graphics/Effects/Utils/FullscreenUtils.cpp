#include "stdafx.h"

#include "FullscreenUtils.h"

#include "Engine/Graphics/SceneGraph/Camera.h"
#include "Engine/Graphics/SceneGraph/TriangleStrip.h"

#include "Engine/Graphics/Resources/VertexDescriptor.h"
#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexArray.h"
#include "Engine/Graphics/Resources/RenderableArrayDataArrays.h"


namespace bv {

namespace {

// **************************
//
VertexDescriptor *  CreateVertexDescriptor( unsigned int numUVChannels )
{
    VertexDescriptor * vd = new VertexDescriptor( 1 + numUVChannels );

    vd->SetAttribute( 0, 0, 0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION );

    // Three position coordinates stored as floats
    unsigned int offset = 3 * sizeof( float );

    for( unsigned int i = 0; i < numUVChannels; ++i )
    {
        vd->SetAttribute( i + 1, i + 1, offset, AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD );
    
        offset += 2 * sizeof( float );
    }

    vd->SetStride( offset );

    return vd;
}

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer *   CreateTriStripArrayData( unsigned int numVertices, unsigned int numUVChannels, float * vbData )
{
    //3 x float (positions) + numUVChannels x 2 x float (uv mapping for all textures)
    unsigned int vertexSize = 3 * sizeof( float ) + 2 * numUVChannels * sizeof( float );

    VertexBuffer * vb       = new VertexBuffer( 4, vertexSize, DataBuffer::Semantic::S_STATIC );    
    VertexDescriptor * vd   = CreateVertexDescriptor( numUVChannels );

    memcpy( vb->Data(), vbData, numVertices * vertexSize );

    VertexArraySingleVertexBuffer * vao = new VertexArraySingleVertexBuffer( vb, vd );
    vao->AddCCEntry( 4 );

    RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );

    return rad;
}

} // anonymous

// **************************
//
Camera *                                        FullscreenUtils::CreateDisplayCamera         ()
{
    //FIXME: upewnic sie, ze tu sa prawdziwe macierze jednostkowe bez zadnych piardow z zaokraglen (i jesli nie sa, to recznie to zapewnic ustawiajac macierze z palca)
    //Ponizszy kod tworzy poprawne macierze identycznosciowe, ale konstruktor domyslnie tez to robi - kod zostaje dla celow szkoleniowych, ale zostawiamy wyniki z konstruktora macierzy w kamerze
    
    //camera->SetFrame( glm::vec3( 0.f, 0.f, 0.f ), glm::vec3( 0.f, 0.f, -1.f ), glm::vec3( 0.f, 1.f, 0.f ) );
    //camera->SetFrustum( -1.f, 1.f, -1.f, 1.f, 1.f, -1.f );

    //FIXME: pewnie mozna na spokojnie te macierze potem poprawic na identycznosciowe (skoro kod do ich generacji jest poporawny i wiadomo, na jakiej zasadzie dziala)
    auto camera = new Camera( false );

    return camera;
}

// **************************
//
TriangleStrip *                                 FullscreenUtils::CreateFullscreenQuad        ( RenderableEffectPtr effect, unsigned int numUVChannels )
{
    float * vbData = CreateFullscreenQuadVBData( numUVChannels );
    
    auto rad = CreateTriStripArrayData( 4, numUVChannels, vbData );
    auto ret = new TriangleStrip( rad, effect );

    delete[] vbData;

    ret->SetWorldTransform( Transform( glm::mat4( 1.0f ), glm::mat4( 1.0f ) ) );

    return ret;
}

// **************************
//
glm::vec3                                       FullscreenUtils::TopLeftPosVertex           ()
{
    return glm::vec3( -1.0f, 1.0f, 0.0f );
}

// **************************
//
glm::vec3                                       FullscreenUtils::TopRightPosVertex          ()
{
    return glm::vec3( 1.0f, 1.0f, 0.0f );
}

// **************************
//
glm::vec3                                       FullscreenUtils::BottomLeftPosVertex        ()
{
    return glm::vec3( -1.0f, -1.0f, 0.0f );
}

// **************************
//
glm::vec3                                       FullscreenUtils::BottomRightPosVertex       ()
{
    return glm::vec3( 1.0f, -1.0f, 0.0f );
}

// **************************
//
glm::vec2                                       FullscreenUtils::TopLeftUVData              ()
{
    return glm::vec2( 0.f, 1.f );
}

// **************************
//
glm::vec2                                       FullscreenUtils::TopRightUVData             ()
{
    return glm::vec2( 1.f, 1.f );
}

// **************************
//
glm::vec2                                       FullscreenUtils::BottomLeftUVData           ()
{
    return glm::vec2( 0.f, 0.f );
}

// **************************
//
glm::vec2                                       FullscreenUtils::BottomRightUVData          ()
{
    return glm::vec2( 1.f, 0.f );
}

// **************************
//
float *                                         FullscreenUtils::CreateFullscreenQuadVBData ( unsigned int numUVChannels )
{
    unsigned int numEntries = ( 2 * numUVChannels + 3 ) * 4;
    unsigned int stride = 2 * numUVChannels + 3;

    float * vertices = new float[ numEntries ];

    /* 
            2 - TopLeft         3 - TopRight

            0 - BottomLeft      1 - BottomRight

    */

    //Raw data
    glm::vec3 positions[] = { BottomLeftPosVertex(), BottomRightPosVertex(), TopLeftPosVertex(), TopRightPosVertex()  };
    glm::vec2 uv[] = { BottomLeftUVData(), BottomRightUVData(), TopLeftUVData(), TopRightUVData() };

    for( unsigned int i = 0; i < 4; ++i )
    {
        // Vertex position offset
        float * vertexpos = &vertices[ i * stride ];

        // Current position
        glm::vec3 & pos = positions[ i ];

        for( unsigned j = 0; j < 3; ++j )
        {
            vertexpos[ j ] = pos[ j ];
        }

        // Vertex uv mapping offset
        float * vertexuv = &vertices[ i * stride + 3 ];

        // Current mapping
        glm::vec2 & uvm = uv[ i ];

        for( unsigned int k = 0; k < numUVChannels; ++k )
        {
            for( unsigned j = 0; j < 2; ++j )
            {
                vertexuv[ k * numUVChannels + j ] = uvm[ j ];
            }
        }
    }

    return vertices;
}

} //bv
