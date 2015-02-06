#include "glutils.h"

#include <cstdio>

#include <gl\glew.h>


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

    glErr = glGetError();
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
        glErr = glGetError();
    }
    return retCode;

}

// ******************************
//
void GLUtils::DumpGLInfo( bool dumpExtensions ) 
{

    const GLubyte * renderer    = glGetString( GL_RENDERER );
    const GLubyte * vendor      = glGetString( GL_VENDOR );
    const GLubyte * version     = glGetString( GL_VERSION );
    const GLubyte * glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );

    GLint major, minor;
    glGetIntegerv( GL_MAJOR_VERSION, &major );
    glGetIntegerv( GL_MINOR_VERSION, &minor );

    printf( "GL Vendor    : %s\n", vendor );
    printf( "GL Renderer  : %s\n", renderer );
    printf( "GL Version   : %s\n", version );
    printf( "GL Version   : %d.%d\n", major, minor );
    printf( "GLSL Version : %s\n", glslVersion );

    if( dumpExtensions )
    {
        GLint nExtensions;

        glGetIntegerv( GL_NUM_EXTENSIONS, &nExtensions );

        for( int i = 0; i < nExtensions; i++ )
        {
            printf( "%s\n", glGetStringi( GL_EXTENSIONS, i ) );
        }
    }
}

// ******************************
//FIXME: add other caps as well
void GLUtils::DumpGLInfoCaps      ()
{
    GLint r0, r1, r2, r3, r4, r5, r6, r7, r8, r9;

    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS,  &r0 );
    glGetIntegerv( GL_MAX_TEXTURE_UNITS,   &r1 );
    glGetIntegerv( GL_MAX_TEXTURE_SIZE,    &r2 );
    glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &r3 );
    glGetIntegerv( GL_MAX_DRAW_BUFFERS,    &r4 );
    glGetIntegerv( GL_MAX_VARYING_FLOATS,  &r5 );

    glGetIntegerv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &r6 );
    glGetIntegerv( GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &r7 );
    glGetIntegerv( GL_MAX_TEXTURE_IMAGE_UNITS, &r8 );
    glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &r9 );

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
