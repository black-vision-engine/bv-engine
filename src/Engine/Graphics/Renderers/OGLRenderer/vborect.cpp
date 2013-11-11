#include "vborect.h"

#include <cstdio>
#include <cmath>

#include "Mathematics/defines.h"

#include "Engine/Graphics/Renderers/OGLRenderer/oglcore.h"
#include "Engine/Graphics/Renderers/OGLRenderer/glutils.h"


namespace bv {

// ********************************
//
VBORect::VBORect( float w, float h )
{
	float wh = w / 2.0f;
	float hh = h / 2.0f;

	float v[] =
    {
		-wh, -hh, 0.f, 
		 wh, -hh, 0.f, 
		-wh,  hh, 0.f, 
		 wh,  hh, 0.f,
	};

 //   float uv[] =
 //   {
 //       0.f, 0.f,
	//    1.f, 0.f,
	//	0.f, 1.f,
 //       1.f, 1.f,
	//};

	unsigned int handle[2];
	glGenBuffers(1, handle);

	glGenVertexArrays( 1, &m_VaoHandle );
    glBindVertexArray( m_VaoHandle );

    glEnableVertexAttribArray( 0 );  // Vertex position
    //glEnableVertexAttribArray( 1 );  // tex pos

    glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), v, GL_STATIC_DRAW);
    glVertexAttribPointer( (GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );

    //glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
    //glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), uv, GL_STATIC_DRAW);
    //glVertexAttribPointer( (GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, ((GLubyte *)NULL + (0)) );
}

// ********************************
//
void VBORect::Render() const {
    GLUtils::CheckForOpenGLError(__FILE__,__LINE__);
    glBindVertexArray( m_VaoHandle );
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	GLUtils::CheckForOpenGLError(__FILE__,__LINE__);
}

} //bv
