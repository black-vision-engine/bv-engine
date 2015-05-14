#pragma once

#include "Engine/Graphics/Types/Transform.h"
#include "Common/GLSLProgram.h"
#include "Prototypes/DrawingCommands/Common/BVGLExt.h"

namespace  bv {

struct DrawingModeData
{
	struct DrawArraysIndirectCommand
	{
		GLuint  count;
		GLuint  primCount;
		GLuint  first;
		GLuint  baseInstance;

		DrawArraysIndirectCommand()
			: count ( 0 )
			, primCount ( 1 )
			, first ( 0 )
			, baseInstance ( 0 )
		{
		}
	};

	struct DrawElementsIndirectCommand
	{
		GLuint  count;
		GLuint  primCount;
		GLuint  first;
		GLuint  baseVertex;
		GLuint  baseInstance;

		DrawElementsIndirectCommand()
			: count ( 0 )
			, primCount ( 1 )
			, first ( 0 )
			, baseVertex ( 0 )
			, baseInstance ( 0 )
		{
		}
	};

	GLenum mode;
	GLsizei count;
	GLsizei primcount;
	GLuint baseinstance;

//indexed
	GLenum type;
	const GLvoid* indices;
	GLint basevertex;

//non-indexed
	GLint first;

//indirect
	DrawArraysIndirectCommand indirectArrays;
	DrawElementsIndirectCommand indirectElements;
	GLsizei drawcount;
	GLsizei stride;

	DrawingModeData()
		: mode( GL_TRIANGLES )
		, count( 0 )
		, primcount( 1 )
		, baseinstance( 0 )
		, type( GL_UNSIGNED_SHORT )
		, indices( 0 )
		, basevertex( 0 )
		, first( 0 )
		, indirectArrays( DrawArraysIndirectCommand () )
		, indirectElements( DrawElementsIndirectCommand() )
		, drawcount( 1 )
		, stride( 0 )
	{
	}
};

class DrawingMode
{
protected:
    GLSLProgram     m_prog;

public:
    static  DrawingMode *		CreateDrawingMode	( unsigned int i);

                                DrawingMode			();
    virtual                     ~DrawingMode		();

	virtual void				Render				( const DrawingModeData& data ) = 0;
};

} //bv
