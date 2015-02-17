#include "GeometryBuilder.h"

#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"


namespace bv {

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer *    GeometryBuilder::CreateRectangle   ( float w, float h, float z, bool withUV )
{
    VertexBuffer * vb     = nullptr;
    VertexDescriptor * vd = nullptr;

    if( withUV )
    {
        vb = new VertexBuffer( 4, ( 2 + 3 ) * sizeof( float ), DataBuffer::Semantic::S_STATIC );
        vd = VertexDescriptor::Create( 2, 0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION,
                                          1, AttributeType::AT_FLOAT2, AttributeSemantic::AS_TEXCOORD );
    }
    else
    {
        vb = new VertexBuffer( 4, 3 * sizeof( float ), DataBuffer::Semantic::S_STATIC );
        vd = VertexDescriptor::Create( 1, 0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION );
    }

    VertexArraySingleVertexBuffer * vao = new VertexArraySingleVertexBuffer( vb, vd );
    vao->AddCCEntry( 4 ); // Single connected component with four vertices

    RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );

    SetupRectTriStrip( vb->Data(), w, h, z, withUV );

    return rad;
}

// **************************
//
void                                             GeometryBuilder::SetupRectTriStrip   ( char * bufChar, float w, float h, float z, bool addUV )
{
    float * buf = (float *) bufChar;

    float v[ 4 * 3 ] =  { -w / 2.f, -h / 2.f, z,
                           w / 2.f, -h / 2.f, z,
                          -w / 2.f,  h / 2.f, z,
                           w / 2.f,  h / 2.f, z };

    float tx[ 4 * 2 ] = { 0.f, 0.f,
                          1.f, 0.f,
                          0.f, 1.f,
                          1.f, 1.f };


    if( addUV )
    {
        for( unsigned int i = 0; i < 4; ++i )
        {
            float * vertex = &buf[ 5 * i ];

            // Position
            vertex[ 0 ] = v[ i + 0 ];
            vertex[ 1 ] = v[ i + 1 ];
            vertex[ 2 ] = v[ i + 2 ];

            vertex[ 3 ] = tx[ i + 0 ];
            vertex[ 4 ] = tx[ i + 1 ];
        }
    }
    else
    {
       memcpy( buf, v, 4 * 3 * sizeof( float ) );
    }
}

}
// bv


#if 0
    // Single attribute channel
    VertexDescriptor * vd   = new VertexDescriptor( 1 ); // FIXME: one attribute for now, later on uv mapping may be added
    vd->SetAttribute( 0, 0, 0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION ); // FIXME: one attribut non-interleaved for now, later on additional channels may be added
    vd->SetStride( 0 ); // exception here - zero stride means that data is tightly packed, but it should be set to the size of a single entry (vec3 in this case)
#endif
