#include "DrawingTestScene0.h"

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

	BVGL::bvglGenVertexArrays( 1, &m_vaoHandle );
	BVGL::bvglGenBuffers( 1, &m_vboHandle );
	BVGL::bvglBindVertexArray(m_vaoHandle);

	BVGL::bvglBindBuffer( GL_ARRAY_BUFFER, m_vboHandle );
    BVGL::bvglBufferData( GL_ARRAY_BUFFER, sizeof( positions ) 
											+ sizeof(instance_colors) 
											+ sizeof(instance_positions),
											NULL, GL_STATIC_DRAW );

	GLuint offset = 0;
	BVGL::bvglBufferSubData( GL_ARRAY_BUFFER, offset, sizeof( positions ), positions );
	offset += sizeof( positions );
    BVGL::bvglBufferSubData( GL_ARRAY_BUFFER, offset, sizeof( instance_colors ), instance_colors );
	offset += sizeof( instance_colors );
    BVGL::bvglBufferSubData( GL_ARRAY_BUFFER, offset, sizeof( instance_positions ), instance_positions );
	offset += sizeof( instance_positions );

    BVGL::bvglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
	BVGL::bvglVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof( positions ) );
	BVGL::bvglVertexAttribPointer( 2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(sizeof( positions ) +
                                 sizeof( instance_colors )) );

    BVGL::bvglEnableVertexAttribArray( 0 );
    BVGL::bvglEnableVertexAttribArray( 1 );
    BVGL::bvglEnableVertexAttribArray( 2 );
	
	BVGL::bvglVertexAttribDivisor( 1, 1 );
	BVGL::bvglVertexAttribDivisor( 2, 1 );

	BVGL::bvglGenBuffers( 1, &m_indexBuffer );

	BVGL::bvglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer );
	BVGL::bvglBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

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

	BVGL::bvglGenBuffers( 1, &m_indirectArraysBuffer );
	BVGL::bvglBindBuffer( GL_DRAW_INDIRECT_BUFFER, m_indirectArraysBuffer );
	BVGL::bvglBufferData( GL_DRAW_INDIRECT_BUFFER, 
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

	BVGL::bvglGenBuffers( 1, &m_indirectElementsBuffer );
	BVGL::bvglBindBuffer( GL_DRAW_INDIRECT_BUFFER, m_indirectElementsBuffer );
	BVGL::bvglBufferData( GL_DRAW_INDIRECT_BUFFER, 
		sizeof( indirectElements ), 
		&indirectElements, GL_STATIC_READ );

    BVGL::bvglBindVertexArray( 0 );

	PrepareShader( "default", "default" );
}

// *************************
//
const char *		DrawingTestScene0::GetName					()
{
	return "DrawingTestScene0";
}

} //bv
