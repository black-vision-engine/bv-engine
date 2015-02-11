#include "GeometryBuilder.h"

#include "Engine/Graphics/Resources/VertexBuffer.h"
#include "Engine/Graphics/Resources/VertexDescriptor.h"


namespace bv {

// **************************
//
RenderableArrayDataArraysSingleVertexBuffer *    GeometryBuilder::CreatreRectangle( float w, float h )
{
    VertexBuffer * vb       = new VertexBuffer( 4, 3 * sizeof( float ), DataBuffer::Semantic::S_STATIC );
    
    // Utility call
    VertexDescriptor * vd = VertexDescriptor::Create( 1, 0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION );

#if 0
    // Equivalent to the call below
    VertexDescriptor * vd   = new VertexDescriptor( 1 ); // FIXME: one attribute for now, later on uv mapping may be added
    vd->SetAttribute( 0, 0, 0, AttributeType::AT_FLOAT3, AttributeSemantic::AS_POSITION ); // FIXME: one attribut non-interleaved for now, later on additional channels may be added
    vd->SetStride( 0 ); // exception here - zero stride means that data is tightly packed, but it should be set to the size of a single entry (vec3 in this case)
#endif
    VertexArraySingleVertexBuffer * vao = new VertexArraySingleVertexBuffer( vb, vd );
    RenderableArrayDataArraysSingleVertexBuffer * rad = new RenderableArrayDataArraysSingleVertexBuffer( vao );
    
    //FIXME: THIS SHIT SHOULD BE SERVICED VIA VERTEX BUFFER DATA ACCESSOR !!!!!!!!!!!!!!! KURWA :P O KURWA TAK
    float * vbData = (float*) vb->Data(); 
    SetUpRectTriStrip( vbData, w, h );

    // FIXME: fill this data with valid 
    return rad;
}

// **************************
//
void                                             GeometryBuilder::SetUpRectTriStrip   ( float * buf, float w, float h )
{
    float v[ 4 * 3 ] =  { -w / 2.f, -h / 2.f, 0.f,
                           w / 2.f, -h / 2.f, 0.f,
                          -w / 2.f,  h / 2.f, 0.f,
                           w / 2.f,  h / 2.f, 0.f };

    memcpy( buf, v, 4 * 3 * sizeof( float ) );
}

}
// bv
