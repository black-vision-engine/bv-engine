#pragma once

#include "oglcore.h"

#include <string>
#include <map>
using std::string;

#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;

namespace bv {

class PixelShader;
class VertexShader;
class GeometryShader;

namespace GLSLShader {
    enum GLSLShaderType {
        VERTEX, FRAGMENT, GEOMETRY,
        TESS_CONTROL, TESS_EVALUATION
    };
};

class GLSLProgram
{
private:
    int			m_Handle;
    bool        m_Compiled;
    bool		m_Linked;
    string		m_LogString;

    int  GetUniformLocation				( const char * name );
    bool FileExists						( const string & fileName );

public:

    GLSLProgram							();
    GLSLProgram							( const PixelShader& ps, const VertexShader& vs, const GeometryShader * gs );
    ~GLSLProgram					    ();

    bool   CompileShaderFromFile		( const char * fileName, GLSLShader::GLSLShaderType type );
    bool   CompileShaderFromString		( const string & source, GLSLShader::GLSLShaderType type );
    bool   Link							();
    bool   Validate						();
    void   Use							();

    string Log                          () const;

    int    GetHandle                    () const;
    bool   IsLinked                     () const;
    bool   IsCompiled                   () const;

    void   BindAttribLocation			( GLuint location, const char * name );
    void   BindFragDataLocation			( GLuint location, const char * name );

    void   SetUniform					( const char *name, float x, float y, float z );
    void   SetUniform					( const char *name, const vec2 & v );
    void   SetUniform					( const char *name, const vec3 & v );
    void   SetUniform					( const char *name, const vec4 & v );
    void   SetUniform					( const char *name, const mat4 & m );
    void   SetUniform					( const char *name, const mat3 & m );
    void   SetUniform					( const char *name, float val );
    void   SetUniform					( const char *name, int val );
    void   SetUniform					( const char *name, bool val );

    void   PrintActiveUniforms();
    void   PrintActiveAttribs();
};

class PixelShader;
class VertexShader;

class GLSLPrograms
{
private:
    std::map<std::pair<PixelShader*, VertexShader*>, GLSLProgram*> programs;

public:
    GLSLProgram* getProgram(PixelShader* ps, VertexShader* vs);

    static GLSLPrograms& get();
};

} //bv
