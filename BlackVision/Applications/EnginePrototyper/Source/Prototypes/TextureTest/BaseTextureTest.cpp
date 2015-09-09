#include "BaseTextureTest.h"


#include "TextureTests\Texture1DTest.h"
#include "TextureTests\Texture2DTest.h"
#include "TextureTests\Texture3DTest.h"
#include "TextureTests\Texture1DArrayTest.h"
#include "TextureTests\Texture2DArrayTest.h"

namespace bv {

// *************************
//
BaseTextureTest::~BaseTextureTest()
{
}

// *************************
//
GLuint					BaseTextureTest::Checkboard				( GLubyte * data, unsigned int width, unsigned int height )
{
	for ( unsigned int i = 0; i < width; ++i ) 
	{
		for ( unsigned int j = 0; j < height; ++j ) 
		{
			unsigned int idx = 4 * ( i * height + j );
			unsigned char c = static_cast< unsigned char >( (( ( (i&32)==0 ) ^ ( (j&32)==0 ) ))*255 );
			data[ idx ] = (GLubyte)c;
			data[ idx + 1 ] = (GLubyte)c;
			data[ idx + 2 ] = (GLubyte)c;
			data[ idx + 3 ] = (GLubyte)255;
		}
	}

	GLuint checkboard;
	BVGL::bvglGenTextures( 1, &checkboard );
	BVGL::bvglBindTexture( GL_TEXTURE_2D, checkboard );

	BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	unsigned int numLevels = 1 + ( unsigned int )( log( std::max( ( double )width, ( double )height ) ) / log ( 2.0 ) );
	BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, numLevels );

	BVGL::bvglTexStorage2D( GL_TEXTURE_2D, numLevels, GL_RGBA8, ( GLsizei )width, ( GLsizei )height );
	
	BVGL::bvglTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, ( GLsizei )width, ( GLsizei )height, 
		GL_RGBA, GL_UNSIGNED_BYTE, &data[ 0 ] );

	BVGL::bvglGenerateMipmap( GL_TEXTURE_2D );

	BVGL::bvglBindTexture( GL_TEXTURE_2D, 0 );

	return checkboard;
}

// *****************************
//
void					BaseTextureTest::ClearColor	( GLubyte * data, unsigned int width, unsigned int height, glm::uvec3 color )
{
	for ( unsigned int i = 0; i < width; ++i ) 
	{
		for ( unsigned int j = 0; j <height; ++j ) 
		{
			unsigned int idx = 4 * ( i * height + j );
			data[ idx ] = (GLubyte)color.x;
			data[ idx + 1 ] = (GLubyte)color.y;
			data[ idx + 2 ] = (GLubyte)color.z;
			data[ idx + 3 ] = (GLubyte)255;
		}
	}
}

// *************************
//

std::map<unsigned int, BaseTextureTest*(*)()>	BaseTextureTest::m_tests;

unsigned int				BaseTextureTest::m_currentTest;

// *************************
//
void						BaseTextureTest::Initialize				()
{
	m_tests[0] = &BaseTextureTest::CreateInstance< Texture2DTest >;
	m_tests[1] = &BaseTextureTest::CreateInstance< Texture2DArrayTest >;
	m_tests[2] = &BaseTextureTest::CreateInstance< Texture1DTest >;
	m_tests[3] = &BaseTextureTest::CreateInstance< Texture1DArrayTest >;
	m_tests[4] = &BaseTextureTest::CreateInstance< Texture3DTest >;
	
	m_currentTest = (unsigned int)m_tests.size() - 1;
}

// *************************
//
BaseTextureTest *			BaseTextureTest::GetTest				( unsigned int idx )
{
	return m_tests[ idx ]();
}

} // bv
