#include "glslprogram.h"

#include "glutils.h"

#include <fstream>
using std::ifstream;
using std::ios;

#include <sstream>
using std::ostringstream;

#include <sys/stat.h>

#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/GeometryShader.h"

namespace bv {

// *******************************
//
GLSLProgram::GLSLProgram() 
    : m_Handle( 0 )
    , m_Linked( false )
    , m_Compiled( false )
{ 
}

bool compileAndLinkProgram  ( GLSLProgram & prog, const std::string & vs, const std::string & ps, const std::string & gs )
{
    if( !prog.CompileShaderFromString( vs.c_str(),GLSLShader::VERTEX ) )
    {
        printf( "Vertex shader failed to compile!\n%s", prog.Log().c_str() );
        return false;
    }

    if( !prog.CompileShaderFromString( ps.c_str(),GLSLShader::FRAGMENT ) )
    {
        printf("Fragment shader failed to compile!\n%s", prog.Log().c_str());
        return false;
    }

    if ( gs != "" && !prog.CompileShaderFromString( gs.c_str(), GLSLShader::GEOMETRY ) )
    {
        printf("Fragment shader failed to compile!\n%s", prog.Log().c_str());
        return false;    
    }

    if( !prog.Link() )
    {
        printf("Shader program failed to link!\n%s", prog.Log().c_str());
        return false;
    }

    if( !prog.Validate() )
    {
        printf("Program failed to validate!\n%s", prog.Log().c_str());
        return false;
    }

    return true;
}

// *******************************
//
GLSLProgram::GLSLProgram( const PixelShader& ps, const VertexShader& vs, const GeometryShader * gs )
    : m_Handle(0)
    , m_Linked(false) 
{
    m_Compiled = compileAndLinkProgram( *this, vs.ProgramSource(), ps.ProgramSource(), gs ? gs->ProgramSource() : "" );
    assert( m_Compiled ); //FIXME: this error should be handled in somehow different manner
}

// *******************************
//
GLSLProgram::~GLSLProgram					    ()
{
    glDeleteProgram( m_Handle );
}

// *******************************
//
bool GLSLProgram::CompileShaderFromFile( const string & fileName, GLSLShader::GLSLShaderType type )
{
    if( ! FileExists( fileName ) )
    {
        m_LogString = "File not found.";
        return false;
    }

    if( m_Handle <= 0 )
    {
        m_Handle = glCreateProgram();
        if( m_Handle == 0)
        {
            m_LogString = "Unable to create shader program.";
            return false;
        }
    }

    ifstream inFile( fileName, ios::in );
    if( !inFile ) {
        return false;
    }

    ostringstream code;
    while( inFile.good() )
    {
        int c = inFile.get();
        if( ! inFile.eof() )
            code << (char) c;
    }

    inFile.close();

    return CompileShaderFromString(code.str(), type);
}

// *******************************
//
bool GLSLProgram::CompileShaderFromString( const string & source, GLSLShader::GLSLShaderType type )
{
    if( m_Handle <= 0 )
    {
        m_Handle = glCreateProgram();
        if( m_Handle == 0)
        {
            m_LogString = "Unable to create shader program.";
            return false;
        }
    }

    GLuint shaderHandle = 0;

    switch( type ) {
    case GLSLShader::VERTEX:
        shaderHandle = glCreateShader( GL_VERTEX_SHADER );
        break;
    case GLSLShader::FRAGMENT:
        shaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
        break;
    case GLSLShader::GEOMETRY:
        shaderHandle = glCreateShader( GL_GEOMETRY_SHADER );
        break;
    case GLSLShader::TESS_CONTROL:
        shaderHandle = glCreateShader( GL_TESS_CONTROL_SHADER );
        break;
    case GLSLShader::TESS_EVALUATION:
        shaderHandle = glCreateShader( GL_TESS_EVALUATION_SHADER );
        break;
    default:
        return false;
    }

    const char * c_code = source.c_str();
    glShaderSource( shaderHandle, 1, &c_code, NULL );

    // Compile the shader
    glCompileShader( shaderHandle );

    // Check for errors
    int result;
    glGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
    if( GL_FALSE == result )
    {
        // Compile failed, store log and return false
        int length = 0;
        m_LogString = "";
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length );
        if( length > 0 ) {
            char * c_log = new char[length];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            m_LogString = c_log;
            delete [] c_log;
        }

        return false;
    } 
    else
    {
        // Compile succeeded, attach shader and return true
        glAttachShader( m_Handle, shaderHandle );
        return true;
    }
}

// *******************************
//
bool GLSLProgram::Link()
{
    if( m_Linked ) 
        return true;

    if( m_Handle <= 0 ) 
        return false;

    glLinkProgram( m_Handle );

    int status = 0;

    glGetProgramiv( m_Handle, GL_LINK_STATUS, &status );
    
    if( GL_FALSE == status )
    {
        // Store log and return false
        int length = 0;
        m_LogString = "";

        glGetProgramiv( m_Handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 )
        {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog( m_Handle, length, &written, c_log );
            m_LogString = c_log;
            delete [] c_log;
        }

        return false;
    } 
    else 
    {
        m_Linked = true;
        return m_Linked;
    }
}

// *******************************
//
void GLSLProgram::Use()
{
    if( m_Handle <= 0 || (!m_Linked) )
        return;

    glUseProgram( m_Handle );
}

// *******************************
//
string GLSLProgram::Log() const
{
    return m_LogString;
}

// *******************************
//
int GLSLProgram::GetHandle() const
{
    return m_Handle;
}

// *******************************
//
bool GLSLProgram::IsLinked() const
{
    return m_Linked;
}

// *******************************
//
bool GLSLProgram::IsCompiled() const
{
    return m_Compiled;
}

// *******************************
//
void GLSLProgram::BindAttribLocation( GLuint location, const string & name )
{
    glBindAttribLocation(m_Handle, location, name.c_str() );
}

// *******************************
//
void GLSLProgram::BindFragDataLocation( GLuint location, const string & name )
{
    glBindFragDataLocation(m_Handle, location, name.c_str() );
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, float x, float y, float z )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniform3f( loc, x, y, z );
    } 
    else 
    {
		//todo: uncomment
        //printf( "Uniform: %s not found.\n", name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, const vec3 & v )
{
    this->SetUniform( name, v.x, v.y, v.z );
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, const vec4 & v)
{
    int loc = GetUniformLocation(name);

    if( loc >= 0 )
    {
        glUniform4f(loc,v.x,v.y,v.z,v.w);
    } 
    else 
    {
		//todo: uncomment
        //printf("Uniform: %s not found.\n",name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, const vec2 & v)
{
    int loc = GetUniformLocation(name);

    if( loc >= 0 )
    {
        glUniform2f(loc,v.x,v.y);
    } 
    else 
    {
		//todo: uncomment
        //printf("Uniform: %s not found.\n",name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, const mat2 & m)
{
    int loc = GetUniformLocation(name);

    if( loc >= 0 )
    {
        glUniformMatrix2fv(loc, 1, GL_FALSE, &m[0][0]);
    } 
    else 
    {
		//todo: uncomment
        //printf("Uniform: %s not found.\n",name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, const mat3 & m)
{
    int loc = GetUniformLocation(name);

    if( loc >= 0 )
    {
        glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
    } 
    else 
    {
		//todo: uncomment
        //printf("Uniform: %s not found.\n",name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, const mat4 & m)
{
    int loc = GetUniformLocation(name);

    if( loc >= 0 )
    {
        glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
    } 
    else 
    {
		//todo: uncomment
        //printf("Uniform: %s not found.\n",name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, float val )
{
    int loc = GetUniformLocation(name);

    if( loc >= 0 )
    {
        glUniform1f(loc, val);
    } 
    else 
    {
		//todo: uncomment
        //printf("Uniform: %s not found.\n",name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, int val )
{
    int loc = GetUniformLocation(name);

    if( loc >= 0 )
    {
        glUniform1i(loc, val);
    } 
    else 
    {
		//todo: uncomment
        //printf("Uniform: %s not found.\n",name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::SetUniform( const string & name, bool val )
{
    int loc = GetUniformLocation(name);

    if( loc >= 0 )
    {
        glUniform1i(loc, val);
    } 
    else 
    {
		//todo: uncomment
        //printf("Uniform: %s not found.\n",name.c_str() );
    }
}

// *******************************
//
void GLSLProgram::PrintActiveUniforms()
{

    GLint nUniforms, size, location, maxLen;
    GLchar * name;
    GLsizei written;
    GLenum type;

    glGetProgramiv( m_Handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen);
    glGetProgramiv( m_Handle, GL_ACTIVE_UNIFORMS, &nUniforms);

    name = (GLchar *) malloc( maxLen );

    printf(" Location | Name\n");
    printf("------------------------------------------------\n");

    for( int i = 0; i < nUniforms; ++i )
    {
        glGetActiveUniform( m_Handle, i, maxLen, &written, &size, &type, name );
        location = glGetUniformLocation( m_Handle, name );

        printf(" %-8d | %s\n",location, name);
    }

    free( name );
}

// *******************************
//
void GLSLProgram::PrintActiveAttribs()
{

    GLint written, size, location, maxLength, nAttribs;
    GLenum type;
    GLchar * name;

    glGetProgramiv( m_Handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength);
    glGetProgramiv( m_Handle, GL_ACTIVE_ATTRIBUTES, &nAttribs);

    name = (GLchar *) malloc( maxLength );

    printf(" Index | Name\n");
    printf("------------------------------------------------\n");

    for( int i = 0; i < nAttribs; i++ )
    {
        glGetActiveAttrib( m_Handle, i, maxLength, &written, &size, &type, name );
        location = glGetAttribLocation( m_Handle, name);
        printf(" %-5d | %s\n",location, name);
    }

    free(name);
}

// *******************************
//
bool GLSLProgram::Validate()
{
    if( !IsLinked() ) 
        return false;

    GLint status;
    glValidateProgram( m_Handle );
    glGetProgramiv( m_Handle, GL_VALIDATE_STATUS, &status );

    if( GL_FALSE == status ) {
        // Store log and return false
        int length = 0;
        m_LogString = "";

        glGetProgramiv( m_Handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 )
        {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(m_Handle, length, &written, c_log);
            m_LogString = c_log;
            delete [] c_log;
        }

        return false;
    } 
    else 
    {
       return true;
    }
}

// *******************************
//
int GLSLProgram::GetUniformLocation( const string & name )
{
    auto it = m_uniformLocations.find( name );
    
    if ( it == m_uniformLocations.end() )
    {
        int loc = glGetUniformLocation( m_Handle, name.c_str() );
        m_uniformLocations[ name ] = loc;

        return loc;
    }

    return it->second;
}

// *******************************
//
bool GLSLProgram::FileExists( const string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat(fileName.c_str(), &info);
    return 0 == ret;
}

//********************** GLSLPrograms ******************//

GLSLProgram* GLSLPrograms::getProgram(PixelShader* ps, VertexShader* vs)
{
    assert(ps && vs);
    auto it = programs.find(std::make_pair(ps,vs));

    if(it != programs.end())
        return it->second;
    else
    {
        auto ret = new GLSLProgram( *ps,*vs, nullptr );
        programs[std::make_pair(ps,vs)] = ret;
        return ret;
    }
}

GLSLPrograms& GLSLPrograms::get()
{
    static GLSLPrograms& instance = GLSLPrograms();

    return instance;
}

} //bv
