#pragma once

#include <gl/glew.h>


namespace bv {

class BVGL
{
public:

    static  GLuint  glCreateProgram ();
    static  void    glDeleteProgram ( GLuint program );

    static  GLuint  glCreateShader  ( GLenum type );
    static  void    glDeleteShader  ( GLuint shader );

};

} //bv
