#include "DrawingTestScene0.h"

#include "Prototypes/DrawingCommands/Common/BVGLExt.h"

namespace bv {

// *************************
//
	DrawingTestScene0::DrawingTestScene0			()
{
	const GLfloat positions[] =
	{
		0.01f	,	0.3f	,	0.0f,
		0.01f	,	0.01f	,	0.0f,
		0.3f	,	0.01f	,	0.0f, 

		-0.01f	,	0.3f	,	0.0f,
		-0.3f	,	0.01f	,	0.0f, 
		-0.01f	,	0.01f	,	0.0f,

		-0.01f	,	-0.3f	,	0.0f,
		-0.01f	,	-0.01f	,	0.0f,
		-0.3f	,	-0.01f	,	0.0f, 
		 
		0.01f	,	-0.3f	,	0.0f,
		0.3f	,	-0.01f	,	0.0f, 
		0.01f	,	-0.01f	,	0.0f,
	};

	const GLushort indices[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
	};

	const GLfloat instance_positions[] =
	{
		-0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 0.0f,
	};

	const GLfloat instance_colors[] =
    {
        1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };

	m_drawingModeData.scene = this;

	m_drawingModeData.count = 12;
	m_drawingModeData.mode = GL_TRIANGLES;

	m_drawingModeData.primcount = 4;
	m_drawingModeData.basevertex = 3;
	m_drawingModeData.baseinstance = 2;

	BVGLExt::bvglGenVertexArrays( 1, &m_vaoHandle );
	BVGLExt::bvglGenBuffers( 1, &m_vboHandle );
	BVGLExt::bvglBindVertexArray(m_vaoHandle);

	BVGLExt::bvglBindBuffer( GL_ARRAY_BUFFER, m_vboHandle );
    BVGLExt::bvglBufferData( GL_ARRAY_BUFFER, sizeof( positions ) 
											+ sizeof(instance_colors) 
											+ sizeof(instance_positions),
											NULL, GL_STATIC_DRAW );

	GLuint offset = 0;
	BVGLExt::bvglBufferSubData( GL_ARRAY_BUFFER, offset, sizeof( positions ), positions );
	offset += sizeof( positions );
    BVGLExt::bvglBufferSubData( GL_ARRAY_BUFFER, offset, sizeof( instance_colors ), instance_colors );
	offset += sizeof( instance_colors );
    BVGLExt::bvglBufferSubData( GL_ARRAY_BUFFER, offset, sizeof( instance_positions ), instance_positions );
	offset += sizeof( instance_positions );

    BVGLExt::bvglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	BVGLExt::bvglVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof( positions ) );
	BVGLExt::bvglVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof( positions ) +
                                 sizeof( instance_colors )) );

    BVGLExt::bvglEnableVertexAttribArray( 0 );
    BVGLExt::bvglEnableVertexAttribArray( 1 );
    BVGLExt::bvglEnableVertexAttribArray( 2 );
	
	BVGLExt::bvglVertexAttribDivisor( 1, 1 );
	BVGLExt::bvglVertexAttribDivisor( 2, 1 );

	BVGLExt::bvglGenBuffers( 1, &m_indexBuffer );

	BVGLExt::bvglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer );
	BVGLExt::bvglBufferDataOverride( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

	//indirect

	m_drawingModeData.stride = 0;

	//indirect arrays

	DrawArraysIndirectCommand indirectArrays[ 4 ];
	for( int i = 0; i < 4; ++i )
	{
		indirectArrays[ i ].first = m_drawingModeData.first;
		indirectArrays[ i ].count = m_drawingModeData.count;
		indirectArrays[ i ].primCount = 1;
		indirectArrays[ i ].baseInstance = i;
	}

	BVGLExt::bvglGenBuffers( 1, &m_indirectArraysBuffer );
	BVGLExt::bvglBindBuffer( GL_DRAW_INDIRECT_BUFFER, m_indirectArraysBuffer );
	BVGLExt::bvglBufferDataOverride( GL_DRAW_INDIRECT_BUFFER, 
		sizeof( indirectArrays ), 
		&indirectArrays, GL_STATIC_READ );

	//indirect elements

	DrawElementsIndirectCommand indirectElements[ 4 ];
	for( int i = 0; i < 4; ++i )
	{
		indirectElements[ i ].first = m_drawingModeData.first;
		indirectElements[ i ].count = m_drawingModeData.count;
		indirectElements[ i ].primCount = 1;
		indirectElements[ i ].baseInstance = i;
		indirectElements[ i ].baseVertex = 3;
	}

	BVGLExt::bvglGenBuffers( 1, &m_indirectElementsBuffer );
	BVGLExt::bvglBindBuffer( GL_DRAW_INDIRECT_BUFFER, m_indirectElementsBuffer );
	BVGLExt::bvglBufferDataOverride( GL_DRAW_INDIRECT_BUFFER, 
		sizeof( indirectElements ), 
		&indirectElements, GL_STATIC_READ );

    BVGLExt::bvglBindVertexArray( 0 );

	PrepareShader( "default", "default" );
}

// *************************
//
const char *		DrawingTestScene0::GetName					()
{
	return "DrawingTestScene0";
}

} //bv
