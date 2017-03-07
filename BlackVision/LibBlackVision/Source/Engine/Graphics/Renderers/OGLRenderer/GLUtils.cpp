#include "stdafx.h"

#include "glutils.h"

#include <cstdio>

#include "BVGL.h"

#include "UseLoggerLibBlackVision.h"


#include "Memory/MemoryLeaks.h"



namespace bv {

// ******************************
//
GLUtils::GLUtils            () 
{
}

// ******************************
//
int GLUtils::CheckForOpenGLError( const char * file, int line )
{
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    GLenum glErr;
    int    retCode = 0;

    glErr = BVGL::bvglGetError();
    while ( glErr != GL_NO_ERROR )
    {
		const char * message = "";
		switch( glErr )
		{
		case GL_INVALID_ENUM:
			message = "Invalid enum";
			break;
		case GL_INVALID_VALUE:
			message = "Invalid value";
			break;
		case GL_INVALID_OPERATION:
			message = "Invalid operation";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			message = "Invalid framebuffer operation";
			break;
		case GL_OUT_OF_MEMORY:
			message = "Out of memory";
			break;
		default:
			message = "Unknown error";
		}

        LOG_MESSAGE( SeverityLevel::error ) << "glError in file " << file << " @ line " << line << ": " << message;
        retCode = 1;
        glErr = BVGL::bvglGetError();
    }
    return retCode;

}

// ******************************
//
void GLUtils::DumpGLInfo( bool dumpExtensions ) 
{

    const GLubyte * renderer    = BVGL::bvglGetString( GL_RENDERER );
    const GLubyte * vendor      = BVGL::bvglGetString( GL_VENDOR );
    const GLubyte * version     = BVGL::bvglGetString( GL_VERSION );
    const GLubyte * glslVersion = BVGL::bvglGetString( GL_SHADING_LANGUAGE_VERSION );

    GLint major, minor;
    BVGL::bvglGetIntegerv( GL_MAJOR_VERSION, &major );
    BVGL::bvglGetIntegerv( GL_MINOR_VERSION, &minor );

    LOG_MESSAGE( SeverityLevel::info )
        << "GL Vendor    : " << vendor << std::endl
        << "GL Renderer  : " << renderer << std::endl
        << "GL Version   : " << version << std::endl
        << "GL Version   : " << major << "." << minor << std::endl
        << "GLSL Version : " << glslVersion;

    if( dumpExtensions )
    {
        GLint nExtensions;

        BVGL::bvglGetIntegerv( GL_NUM_EXTENSIONS, &nExtensions );

        for( int i = 0; i < nExtensions; i++ )
        {
            LOG_MESSAGE( SeverityLevel::info ) << BVGL::bvglGetStringi( GL_EXTENSIONS, i );
        }
    }
}

// ******************************
//FIXME: add other caps as well
void GLUtils::DumpGLInfoCaps      ()
{
    GLint r0, r1, r2, r3, r4, r5, r6, r7, r8, r9;

    BVGL::bvglGetIntegerv( GL_MAX_VERTEX_ATTRIBS,  &r0 );
    BVGL::bvglGetIntegerv( GL_MAX_TEXTURE_UNITS,   &r1 );
    BVGL::bvglGetIntegerv( GL_MAX_TEXTURE_SIZE,    &r2 );
    BVGL::bvglGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &r3 );
    BVGL::bvglGetIntegerv( GL_MAX_DRAW_BUFFERS,    &r4 );
    BVGL::bvglGetIntegerv( GL_MAX_VARYING_FLOATS,  &r5 );

    BVGL::bvglGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &r6 );
    BVGL::bvglGetIntegerv( GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &r7 );
    BVGL::bvglGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &r8 );
    BVGL::bvglGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &r9 );

    LOG_MESSAGE( SeverityLevel::info ) 
        <<  "Max vertex attribs:       " << r0 << std::endl
        <<  "Max texture units fixed:  " << r1 << std::endl
        <<  "Max texture size:         " << r2 << std::endl 
        <<  "Max 3D texture size:      " << r3 << std::endl
        <<  "Max draw buffers:         " << r4 << std::endl
        <<  "Max varying floats:       " << r5 << std::endl

        << std::endl

        << "Max vertex texture image units:    " << r6 << std::endl
        << "Max geometry texture image units:  " << r7 << std::endl
        << "Max texture image units:           " << r8 << std::endl
        << "Max combined texture image units:  " << r9 << std::endl;

}

} //bv
