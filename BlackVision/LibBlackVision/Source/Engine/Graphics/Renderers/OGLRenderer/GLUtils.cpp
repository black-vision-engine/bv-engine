#include "stdafx.h"

#include "glutils.h"

#include <cstdio>

#include "BVGL.h"


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

        printf("glError in file %s @ line %d: %s\n", file, line, message);
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

    printf( "GL Vendor    : %s\n", vendor );
    printf( "GL Renderer  : %s\n", renderer );
    printf( "GL Version   : %s\n", version );
    printf( "GL Version   : %d.%d\n", major, minor );
    printf( "GLSL Version : %s\n", glslVersion );

    if( dumpExtensions )
    {
        GLint nExtensions;

        BVGL::bvglGetIntegerv( GL_NUM_EXTENSIONS, &nExtensions );

        for( int i = 0; i < nExtensions; i++ )
        {
            printf( "%s\n", BVGL::bvglGetStringi( GL_EXTENSIONS, i ) );
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

    printf( "Max vertex attribs:       %d\n", r0 );
    printf( "Max texture units fixed:  %d\n", r1 );
    printf( "Max texture size:         %d\n", r2 );
    printf( "Max 3D texture size:      %d\n", r3 );
    printf( "Max draw buffers:         %d\n", r4 );
    printf( "Max varying floats:       %d\n", r5 );

    printf( "\n" );

    printf( "Max vertex texture image units:    %d\n", r6 );
    printf( "Max geometry texture image units:  %d\n", r7 );
    printf( "Max texture image units:           %d\n", r8 );
    printf( "Max combined texture image units:  %d\n", r9 );

}

} //bv
