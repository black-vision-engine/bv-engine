#pragma once

#include <map>

#include "Common/PrototyperCore.h"
#include "DrawingMode.h"

namespace bv {

class DrawingTestScene
{
protected:

    GLuint				m_vaoHandle;
    GLuint				m_indexBuffer;
    GLuint				m_vboHandle;
	GLuint				m_indirectArraysBuffer;
	GLuint				m_indirectElementsBuffer;

	DrawingModeData		m_drawingModeData;
    
	GLSLProgram			m_prog;

public:
	virtual								~DrawingTestScene		();

	virtual const DrawingModeData&		GetDrawingModeData		();
    virtual void						Bind					();
    virtual void						BindIndirectArrays		();
    virtual void						BindIndirectElements	();

	virtual const char *				GetName					()		= 0;

protected:
	bool								PrepareShader			( const char * vname, const char * fname);

//-----------------registration ----------------

private:
	static std::map<unsigned int, DrawingTestScene*(*)()>	m_scenes;
	static unsigned int										m_currentScene;
	template<typename T> static DrawingTestScene *			CreateInstance		();

public:
	static	DrawingTestScene *			GetNextScene			();	
	static	void						Initialize				();	

};

} //bv
