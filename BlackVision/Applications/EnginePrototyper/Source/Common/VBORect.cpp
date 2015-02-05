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


    glGenVertexArrays( 1, &m_vaoHandle );
    glBindVertexArray( m_vaoHandle );

    GLuint bufferHandles[ 3 ];

    glGenBuffers( 3, bufferHandles );

    // Vertices
    glBindBuffer( GL_ARRAY_BUFFER, bufferHandles[ 0 ] );
    glBufferData( GL_ARRAY_BUFFER, 4 * 3 * sizeof( float ), v, GL_STATIC_DRAW );
    glVertexAttribPointer( (GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    glEnableVertexAttribArray( 0 );

    // Normals
    glBindBuffer( GL_ARRAY_BUFFER, bufferHandles[ 1 ] );
    glBufferData( GL_ARRAY_BUFFER, 4 * 3 * sizeof( float ), n, GL_STATIC_DRAW );
    glVertexAttribPointer( (GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    glEnableVertexAttribArray( 1 );

    // Texture coordinates
    glBindBuffer( GL_ARRAY_BUFFER, bufferHandles[ 2 ] );
    glBufferData( GL_ARRAY_BUFFER, 4 * 2 * sizeof( float ), tx, GL_STATIC_DRAW );
    glVertexAttribPointer( (GLuint) 2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 );
}

// *************************
//
VBORect::~VBORect               ()
{
    glBindVertexArray( 0 );
    glDeleteVertexArrays( 1, &m_vaoHandle );
}

// *************************
//
void    VBORect::Render         ()
{
    glBindVertexArray( m_vaoHandle );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
}

} //bv
