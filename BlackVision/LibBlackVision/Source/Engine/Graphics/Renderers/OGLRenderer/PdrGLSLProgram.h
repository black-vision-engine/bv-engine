#pragma once

#include <string>
#include <map>
#include <hash_map>

#include "BVGL.h"

#include "Mathematics/glm_inc.h"

using std::string;

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


    enum GLSLShaderType
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        TESS_CONTROL,
        TESS_EVALUATION
    };
};

class PdrGLSLProgram
{
private:

    typedef     std::hash_map< std::string, int >   TUniformLocations;

private:

    TUniformLocations   m_uniformLocations;

    int			        m_Handle;
    bool                m_Compiled;
    bool		        m_Linked;
    
    bool                m_verboseLogging;
    string		        m_LogString;


    inline int  GetUniformLocation		( const string & name );

    bool        FileExists				( const string & fileName ) const;

public:

    PdrGLSLProgram						();
    PdrGLSLProgram				        ( const PixelShader& ps, const VertexShader& vs, const GeometryShader * gs );
    ~PdrGLSLProgram					    ();

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

    inline int    SetUniform            ( const string & name, float x, float y, float z );
    inline int    SetUniform            ( const string & name, const vec2 & v );
    inline int    SetUniform            ( const string & name, const vec3 & v );
    inline int    SetUniform            ( const string & name, const vec4 & v );
    inline int    SetUniform            ( const string & name, const mat2 & m );
    inline int    SetUniform            ( const string & name, const mat3 & m );
    inline int    SetUniform            ( const string & name, const mat4 & m );
    inline int    SetUniform            ( const string & name, float val );
    inline int    SetUniform            ( const string & name, int val );
    inline int    SetUniform            ( const string & name, bool val );

    inline void   SetUniform            ( int loc, float x, float y, float z );
    inline void   SetUniform            ( int loc, const vec2 & v );
    inline void   SetUniform            ( int loc, const vec3 & v );
    inline void   SetUniform            ( int loc, const vec4 & v );
    inline void   SetUniform            ( int loc, const mat2 & m );
    inline void   SetUniform            ( int loc, const mat3 & m );
    inline void   SetUniform            ( int loc, const mat4 & m );
    inline void   SetUniform            ( int loc, float val );
    inline void   SetUniform            ( int loc, int val );
    inline void   SetUniform            ( int loc, bool val );

private:

    inline void   SetUniformImpl        ( int loc, float x, float y, float z );
    inline void   SetUniformImpl        ( int loc, const vec2 & v );
    inline void   SetUniformImpl        ( int loc, const vec3 & v );
    inline void   SetUniformImpl        ( int loc, const vec4 & v );
    inline void   SetUniformImpl        ( int loc, const mat2 & m );
    inline void   SetUniformImpl        ( int loc, const mat3 & m );
    inline void   SetUniformImpl        ( int loc, const mat4 & m );
    inline void   SetUniformImpl        ( int loc, float val );
    inline void   SetUniformImpl        ( int loc, int val );
    inline void   SetUniformImpl        ( int loc, bool val );

    void          PostGetUniformLocationFail    ( const string & name );
    void          PostGetUniformLocationFail    ( int loc );

public:

    void   PrintActiveUniforms          ();
    void   PrintActiveAttribs           ();
};

class PixelShader;
class VertexShader;

class PdrGLSLPrograms
{
private:

    std::map<std::pair< PixelShader *, VertexShader * >, PdrGLSLProgram * > programs;

public:

    PdrGLSLProgram *            getProgram( PixelShader * ps, VertexShader * vs );

    static PdrGLSLPrograms &    get();
};

} //bv

#include "PdrGLSLProgram.inl"
