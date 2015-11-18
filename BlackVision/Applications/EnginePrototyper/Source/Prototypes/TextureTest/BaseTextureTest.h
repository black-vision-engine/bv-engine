#pragma once

#include <map>

#include "Common/GLSLProgram.h"
#include "Common/VBORect.h"

namespace bv {

class Renderer;

namespace semantic
{
	namespace attr
	{
		enum type : int
		{
			POSITION = 0,
			NORMAL,
			TEXCOORD,
		};
	};
};

class BaseTextureTest
{
protected:
	GLSLProgram			m_program;

public:
	virtual			~BaseTextureTest();

    virtual     void    Initialize	( Renderer *, glm::mat4 mvp ) = 0;
    virtual     void    Render      () = 0;
    virtual     void    Update      ( TimeType t ) = 0;

protected:
	void				PrepareShader( const std::string & vs, const std::string & fs );
	
	GLuint				Checkboard	( GLubyte * data, unsigned int width, unsigned int height );
	void				ClearColor	( GLubyte * data, unsigned int width, unsigned int height, glm::uvec3 color );

//-----------------registration ----------------

private:
	static std::map< unsigned int, BaseTextureTest*(*)() >	m_tests;
	static unsigned int										m_currentTest;
	template< typename T > static BaseTextureTest *			CreateInstance		();

public:
	static	void						Initialize			();	
	static	BaseTextureTest *			GetTest				( unsigned int idx );	
};

// *************************
//
template<typename T> 
BaseTextureTest *			BaseTextureTest::CreateInstance()
{
	return new T;
}


} //bv
