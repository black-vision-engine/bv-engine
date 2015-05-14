#include "DrawingTestScene1.h"

#include "Prototypes/DrawingCommands/Common/BVGLExt.h"

namespace bv {

// *************************
//
							DrawingTestScene1::DrawingTestScene1			()
{
	const GLfloat positions[] =
	{
		1,1,0,
		-1,1,0,
		1,-1,0
	};

	const GLushort indices[] =
	{
		0, 1, 2
	};

	BVGLExt::bvglGenVertexArrays( 1, &m_vaoHandle );
	BVGLExt::bvglBindVertexArray(m_vaoHandle);

    GLuint vbo[1];
	BVGLExt::bvglGenBuffers( 1, vbo );

	BVGLExt::bvglBindBuffer( GL_ARRAY_BUFFER, vbo[0] );
    BVGLExt::bvglBufferData( GL_ARRAY_BUFFER, sizeof( positions ), positions, GL_STATIC_DRAW );
    BVGLExt::bvglVertexAttribPointer( (GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)(0) );
    BVGLExt::bvglEnableVertexAttribArray( 0 );

	BVGLExt::bvglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbo[ 0 ] );
	BVGLExt::bvglBufferDataOverride( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    BVGLExt::bvglBindVertexArray( 0 );

	m_drawingModeData.mode = GL_TRIANGLES;
	m_drawingModeData.count = 3;
	m_drawingModeData.type = GL_UNSIGNED_SHORT;
	m_drawingModeData.indices = 0;
}

} //bv
