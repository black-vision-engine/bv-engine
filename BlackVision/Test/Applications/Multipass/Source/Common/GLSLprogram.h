#pragma once

#include <string>

#include "Common/PrototyperCore.h"

namespace bv {

class GLSLProgram
{
private:

    GLint       m_handle;
    bool        m_linked;

    std::string m_logString;

    GLint   GetUniformLocation  ( const char * name );
    bool    FileExists          ( const std::string & fileName );

public:

    GLSLProgram     ();
    ~GLSLProgram    ();

    bool   CompileShaderFromFile( const char * fileName, GLuint shaderType );
    bool   CompileShaderFromString( const std::string & source, GLuint shaderType );
    bool   Link();
    bool   Validate();
    void   Use();

    std::string Log() const;

    GLint  GetHandle();
    bool   IsLinked();

    void   BindAttribLocation( GLuint location, const char * name);
    void   BindFragDataLocation( GLuint location, const char * name );

    void   SetUniform( const char * name, float x, float y, float z );
    void   SetUniform( const char * name, const glm::vec2 & v );
    void   SetUniform( const char * name, const glm::vec3 & v );
    void   SetUniform( const char * name, const glm::vec4 & v );
    void   SetUniform( const char * name, const glm::mat4 & m );
    void   SetUniform( const char * name, const glm::mat3 & m );
    void   SetUniform( const char * name, float val );
    void   SetUniform( const char * name, int val );
    void   SetUniform( const char * name, bool val );

    void   PrintActiveUniforms();
    void   PrintActiveAttribs();

};

} // bv