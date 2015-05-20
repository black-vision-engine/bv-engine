#pragma once

#include <map>

#include "Engine/Graphics/Types/Transform.h"
#include "Common/GLSLProgram.h"

namespace  bv {

class DrawingTestScene;

struct DrawArraysIndirectCommand
{
	GLuint  count;
	GLuint  primCount;
	GLuint  first;
	GLuint  baseInstance;

	DrawArraysIndirectCommand();
};

struct DrawElementsIndirectCommand
{
	GLuint  count;
	GLuint  primCount;
	GLuint  first;
	GLuint  baseVertex;
	GLuint  baseInstance;

	DrawElementsIndirectCommand();
};

struct DrawingModeData
{
	GLenum mode;
	GLsizei count;
	GLsizei primcount;
	GLuint baseinstance;

//indexed
	GLenum type;
	GLint basevertex;

//non-indexed
	GLint first;

//indirect
	GLsizei stride;

	DrawingTestScene * scene;

	DrawingModeData();
};

class DrawingMode
{
protected:
    GLSLProgram				m_prog;

public:
									DrawingMode			();
    virtual							~DrawingMode		();

	virtual void					Render				( const DrawingModeData& data ) = 0;
	virtual const char *			GetName				()								= 0;


//-----------------registration ----------------

private:
	static std::map<unsigned char, DrawingMode*(*)()>	m_modes;
	template<typename T> static DrawingMode *			CreateInstance		();

public:
	static	DrawingMode *				GetMode					( unsigned char name );	
	static	void						Initialize				();	

};

} //bv
