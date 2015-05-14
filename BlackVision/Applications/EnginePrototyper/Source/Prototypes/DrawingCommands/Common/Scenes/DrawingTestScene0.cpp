#include "DrawingTestScene0.h"

#include "Prototypes/DrawingCommands/Common/BVGLExt.h"

namespace bv {

// *************************
//
							DrawingTestScene0::DrawingTestScene0			()
{
	const GLfloat positions[] =
	{
		0.01f	,	0.8f	,	0.0f,
		0.01f	,	0.01f	,	0.0f,
		0.7f	,	0.01f	,	0.0f, 

		-0.01f	,	0.8f	,	0.0f,
		-0.7f	,	0.01f	,	0.0f, 
		-0.01f	,	0.01f	,	0.0f,

		-0.01f	,	-0.8f	,	0.0f,
		-0.01f	,	-0.01f	,	0.0f,
		-0.7f	,	-0.01f	,	0.0f, 
		 
		0.01f	,	-0.8f	,	0.0f,
		0.7f	,	-0.01f	,	0.0f, 
		0.01f	,	-0.01f	,	0.0f,
	};

	const GLushort indices[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
	};

	const GLfloat instance_colors[] =
    {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
    };


	m_drawingModeData.count = 12;
	
	m_drawingModeData.mode = GL_TRIANGLES;

	BVGLExt::bvglGenVertexArrays( 1, &m_vaoHandle );
	BVGLExt::bvglGenBuffers( 1, &m_triangleBuffer );
	BVGLExt::bvglBindVertexArray(m_vaoHandle);

	BVGLExt::bvglBindBuffer( GL_ARRAY_BUFFER, m_triangleBuffer );
    BVGLExt::bvglBufferData( GL_ARRAY_BUFFER, sizeof( positions ) + sizeof(instance_colors) , NULL, GL_STATIC_DRAW );

	BVGLExt::bvglBufferSubData( GL_ARRAY_BUFFER, 0, sizeof( positions ), positions );
    BVGLExt::bvglBufferSubData( GL_ARRAY_BUFFER, sizeof( positions ), sizeof( instance_colors ), instance_colors );

    BVGLExt::bvglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	BVGLExt::bvglVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof( positions ) );

    BVGLExt::bvglEnableVertexAttribArray( 0 );
    BVGLExt::bvglEnableVertexAttribArray( 1 );
	
	//BVGLExt::bvglVertexAttribDivisor( 1, 1 );

	BVGLExt::bvglGenBuffers( 1, &m_indexBuffer );

	BVGLExt::bvglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer );
	BVGLExt::bvglBufferDataOverride( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

    BVGLExt::bvglBindVertexArray( 0 );
}

} //bv
