#include "VBOCube.h"

namespace bv {

const unsigned int VBOCube::INDICIES_NUM = 36;

// *************************
//
VBOCube::VBOCube                ( float x )
{
	GLfloat xdiv2 = x / 2.f;
	GLfloat v[ 8 * 3 ] = {
		-xdiv2, -xdiv2,  xdiv2,
		xdiv2, -xdiv2,  xdiv2,
		xdiv2,  xdiv2,  xdiv2,
		-xdiv2,  xdiv2,  xdiv2,
		-xdiv2, -xdiv2, -xdiv2,
		xdiv2, -xdiv2, -xdiv2,
		xdiv2,  xdiv2, -xdiv2,
		-xdiv2,  xdiv2, -xdiv2
	};

	GLfloat tx[ 8 * 3 ] = {
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f
	};

    GLushort idx[ INDICIES_NUM ] = { 
		// front
		0, 1, 2, 2, 3, 0,
		// top
		3, 2, 6, 6, 7, 3,
		// back
		7, 6, 5, 5, 4, 7,
		// bottom
		4, 5, 1, 1, 0, 4,
		// left
		4, 0, 3, 3, 7, 4,
		// right
		1, 5, 6, 6, 2, 1
	};


    BVGL::bvglGenVertexArrays( 1, &m_vaoHandle );
    BVGL::bvglBindVertexArray( m_vaoHandle );

    GLuint bufferHandles[ 3 ];

    BVGL::bvglGenBuffers( 3, bufferHandles );

    // Vertices
    BVGL::bvglBindBuffer( GL_ARRAY_BUFFER, bufferHandles[ 0 ] );
    BVGL::bvglBufferData( GL_ARRAY_BUFFER, 8 * 3 * sizeof( GLfloat ), v, GL_STATIC_DRAW );
    BVGL::bvglVertexAttribPointer( (GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    BVGL::bvglEnableVertexAttribArray( 0 );

	// Texture
    BVGL::bvglBindBuffer( GL_ARRAY_BUFFER, bufferHandles[ 1 ] );
    BVGL::bvglBufferData( GL_ARRAY_BUFFER, 8 * 3 * sizeof( GLfloat ), tx, GL_STATIC_DRAW );
    BVGL::bvglVertexAttribPointer( (GLuint) 2, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    BVGL::bvglEnableVertexAttribArray( 2 );

    // Indecies
    BVGL::bvglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bufferHandles[ 2 ] );
    BVGL::bvglBufferData( GL_ELEMENT_ARRAY_BUFFER, INDICIES_NUM * sizeof( GLushort ), idx, GL_STATIC_DRAW );

    BVGL::bvglBindVertexArray( 0 );
}

// *************************
//
VBOCube::~VBOCube               ()
{
    BVGL::bvglBindVertexArray( 0 );
    BVGL::bvglDeleteVertexArrays( 1, &m_vaoHandle );
}

// *************************
//
void    VBOCube::Render         ()
{
    BVGL::bvglBindVertexArray( m_vaoHandle );
	BVGL::bvglDrawElements (GL_TRIANGLES, INDICIES_NUM, GL_UNSIGNED_SHORT, 0);
}

} //bv
