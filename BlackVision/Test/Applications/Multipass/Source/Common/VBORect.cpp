#include "VBORect.h"

namespace bv {

// *************************
//
VBORect::VBORect                ( float w, float h )
{
    float side_x = w;
    float side_y = h;

    float v[ 4 * 3 ] =  { -side_x / 2.f, -side_y / 2.f, 0.f,
                           side_x / 2.f, -side_y / 2.f, 0.f,
                          -side_x / 2.f,  side_y / 2.f, 0.f,
                           side_x / 2.f,  side_y / 2.f, 0.f };

    float n[ 4 * 3 ] =  { 0.f, 0.f, 1.f,
                          0.f, 0.f, 1.f,
                          0.f, 0.f, 1.f,
                          0.f, 0.f, 1.f };

    float tx[ 4 * 2 ] = { 0.f, 0.f,
                          1.f, 0.f,
                          0.f, 1.f,
                          1.f, 1.f };


    BVGL::bvglGenVertexArrays( 1, &m_vaoHandle );
    BVGL::bvglBindVertexArray( m_vaoHandle );

    GLuint bufferHandles[ 3 ];

    BVGL::bvglGenBuffers( 3, bufferHandles );

    // Vertices
    BVGL::bvglBindBuffer( GL_ARRAY_BUFFER, bufferHandles[ 0 ] );
    BVGL::bvglBufferData( GL_ARRAY_BUFFER, 4 * 3 * sizeof( float ), v, GL_STATIC_DRAW );
    BVGL::bvglVertexAttribPointer( (GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    BVGL::bvglEnableVertexAttribArray( 0 );

    // Normals
    BVGL::bvglBindBuffer( GL_ARRAY_BUFFER, bufferHandles[ 1 ] );
    BVGL::bvglBufferData( GL_ARRAY_BUFFER, 4 * 3 * sizeof( float ), n, GL_STATIC_DRAW );
    BVGL::bvglVertexAttribPointer( (GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    BVGL::bvglEnableVertexAttribArray( 1 );

    // Texture coordinates
    BVGL::bvglBindBuffer( GL_ARRAY_BUFFER, bufferHandles[ 2 ] );
    BVGL::bvglBufferData( GL_ARRAY_BUFFER, 4 * 2 * sizeof( float ), tx, GL_STATIC_DRAW );
    BVGL::bvglVertexAttribPointer( (GLuint) 2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    BVGL::bvglEnableVertexAttribArray( 2 );

    BVGL::bvglBindVertexArray( 0 );
}

// *************************
//
VBORect::~VBORect               ()
{
    BVGL::bvglBindVertexArray( 0 );
    BVGL::bvglDeleteVertexArrays( 1, &m_vaoHandle );
}

// *************************
//
void    VBORect::Render         ()
{
    BVGL::bvglBindVertexArray( m_vaoHandle );
    BVGL::bvglDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

} //bv
