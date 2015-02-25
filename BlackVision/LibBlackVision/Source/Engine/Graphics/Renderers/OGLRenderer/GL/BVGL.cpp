#include "BVGL.h"

// #include <cstdio>


namespace bv {

// *****************************
//
GLuint  BVGL::glCreateProgram ()
{
    return ::glCreateProgram();
}

// *****************************
//
void    BVGL::glDeleteProgram ( GLuint program )
{
    ::glDeleteProgram( program );
}

// *****************************
//
GLuint  BVGL::glCreateShader  ( GLenum type )
{
    return ::glCreateShader( type );
}

// *****************************
//
void    BVGL::glDeleteShader  ( GLuint shader )
{
    ::glDeleteShader( shader );
}

} //bv
