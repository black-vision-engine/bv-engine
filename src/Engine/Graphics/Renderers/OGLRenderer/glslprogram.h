#pragma once

#include "Engine/Graphics/Renderers/OGLRenderer/oglcore.h"

#include <string>
#include <map>
#include <hash_map>

using std::string;

#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat2;
using glm::mat3;
using glm::mat4;


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

    typedef     std::hash_map< std::string, int >   TUniformLocations;

private:

    TUniformLocations   m_uniformLocations;

    int			        m_Handle;
    bool                m_Compiled;
    bool		        m_Linked;
    string		        m_LogString;

    int  GetUniformLocation				( const string & name );
    bool FileExists						( const string & fileName );

public:

    GLSLProgram							();
    GLSLProgram							( const PixelShader& ps, const VertexShader& vs, const GeometryShader * gs );
    ~GLSLProgram					    ();

    bool   CompileShaderFromFile		( const string & fileName, GLSLShader::GLSLShaderType type );
    bool   CompileShaderFromString		( const string & source, GLSLShader::GLSLShaderType type );
    bool   Link							();
    bool   Validate						();
    void   Use							();

    string Log                          () const;

    int    GetHandle                    () const;
    bool   IsLinked                     () const;
    bool   IsCompiled                   () const;

    void   BindAttribLocation			( GLuint location, const string & name );
    void   BindFragDataLocation			( GLuint location, const string & name );

    void   SetUniform					( const string & name, float x, float y, float z );
    void   SetUniform					( const string & name, const vec2 & v );
    void   SetUniform					( const string & name, const vec3 & v );
    void   SetUniform					( const string & name, const vec4 & v );
    void   SetUniform					( const string & name, const mat2 & m );
    void   SetUniform					( const string & name, const mat3 & m );
    void   SetUniform					( const string & name, const mat4 & m );
    void   SetUniform					( const string & name, float val );
    void   SetUniform					( const string & name, int val );
    void   SetUniform					( const string & name, bool val );

    void   PrintActiveUniforms          ();
    void   PrintActiveAttribs           ();
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
