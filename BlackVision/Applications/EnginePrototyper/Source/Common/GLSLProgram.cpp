#include "glslprogram.h"

#include <fstream>
#include <sstream>

#include <sys/stat.h>

namespace bv {

// **************************
//
GLSLProgram::GLSLProgram() 
    : m_handle( 0 )
    , m_linked( false ) 
{ 

}

// **************************
//
GLSLProgram::~GLSLProgram()
{
    glDeleteProgram( m_handle );    
}

// **************************
//
bool GLSLProgram::CompileShaderFromFile( const char * fileName, GLuint shaderType )
{
    if( !FileExists( fileName ) )
    {
        m_logString = "File not found.";
        
        return false;
    }

    if( m_handle <= 0 ) {
        m_handle = glCreateProgram();
        
        if( m_handle == 0 )
        {
            m_logString = "Unable to create shader program.";

            return false;
        }
    }

    std::ifstream inFile( fileName, std::ios::in );

    if( !inFile )
    {
        return false;
    }

    std::ostringstream src;

    while( inFile.good() )
    {
        int c = inFile.get();
        if( ! inFile.eof() )
            src << (char) c;
    }

    inFile.close();

    return CompileShaderFromString( src.str(), shaderType );
}

// **************************
//
bool GLSLProgram::CompileShaderFromString( const std::string & source, GLuint shaderType )
{
    if( m_handle <= 0 )
    {
        m_handle = glCreateProgram();
        
        if( m_handle == 0 )
        {
            m_logString = "Unable to create shader program.";

            return false;
        }
    }

    GLuint shaderHandle = 0;

    switch( shaderType )
    {
        case GL_VERTEX_SHADER:
            shaderHandle = glCreateShader( GL_VERTEX_SHADER );
            break;
        case GL_FRAGMENT_SHADER:
            shaderHandle = glCreateShader( GL_FRAGMENT_SHADER );
            break;
        case GL_GEOMETRY_SHADER:
            shaderHandle = glCreateShader( GL_GEOMETRY_SHADER );
            break;
        case GL_TESS_CONTROL_SHADER:
            shaderHandle = glCreateShader( GL_TESS_CONTROL_SHADER );
            break;
        case GL_TESS_EVALUATION_SHADER:
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

        m_logString = "";
        
        glGetShaderiv( shaderHandle, GL_INFO_LOG_LENGTH, &length );
        
        if( length > 0 )
        {
            char * c_log = new char[ length ];
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, c_log);
            
            m_logString = c_log;
            
            delete [] c_log;
        }

        return false;
    } 
    else 
    {
        // Compile succeeded, attach shader and return true
        glAttachShader( m_handle, shaderHandle );

        return true;
    }
}

// **************************
//
bool GLSLProgram::Link()
{
    if( m_linked ) 
    {
        return true;
    }

    if( m_handle <= 0 ) 
    {
        return false;
    }

    glLinkProgram( m_handle );

    int status = 0;

    glGetProgramiv( m_handle, GL_LINK_STATUS, &status );

    if( GL_FALSE == status )
    {
        // Store log and return false
        int length = 0;
        
        m_logString = "";

        glGetProgramiv( m_handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 )
        {
            char * c_log = new char[ length ];
        
            int written = 0;
            
            glGetProgramInfoLog( m_handle, length, &written, c_log );
            
            m_logString = c_log;
            
            delete [] c_log;
        }

        return false;
    } 
    else 
    {
        m_linked = true;

        return m_linked;
    }
}

// **************************
//
void GLSLProgram::Use()
{
    if( m_handle <= 0 || ( !m_linked ) ) 
    {
        return;
    }

    glUseProgram( m_handle );
}

// **************************
//
std::string GLSLProgram::Log() const
{
    return m_logString;
}

// **************************
//
GLint   GLSLProgram::GetHandle()
{
    return m_handle;
}

// **************************
//
bool GLSLProgram::IsLinked()
{
    return m_linked;
}

// **************************
//
void GLSLProgram::BindAttribLocation( GLuint location, const char * name )
{
    glBindAttribLocation( m_handle, location, name );
}

// **************************
//
void GLSLProgram::BindFragDataLocation( GLuint location, const char * name )
{
    glBindFragDataLocation( m_handle, location, name);
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, float x, float y, float z )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniform3f( loc, x, y, z );
    }
    else
    {
        printf( "Uniform: %s not found.\n", name );
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, const glm::vec3 & v )
{
    SetUniform( name, v.x, v.y, v.z );
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, const glm::vec4 & v )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniform4f( loc, v.x, v.y, v.z, v.w );
    }
    else
    {
        printf( "Uniform: %s not found.\n", name );
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, const glm::vec2 & v )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniform2f( loc, v.x, v.y );
    }
    else
    {
        printf( "Uniform: %s not found.\n", name );
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, const glm::mat4 & m )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniformMatrix4fv( loc, 1, GL_FALSE, &m[0][0] );
    }
    else
    {
        printf( "Uniform: %s not found.\n", name );
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, const glm::mat3 & m )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniformMatrix3fv( loc, 1, GL_FALSE, &m[0][0] );
    }
    else
    {
        printf( "Uniform: %s not found.\n", name );
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, float val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniform1f( loc, val );
    }
    else
    {
        printf( "Uniform: %s not found.\n", name );
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, int val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniform1i( loc, val );
    }
    else
    {
        printf( "Uniform: %s not found.\n", name );
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, bool val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        glUniform1i( loc, val );
    }
    else
    {
        printf( "Uniform: %s not found.\n", name );
    }
}

// **************************
//
void GLSLProgram::PrintActiveUniforms()
{

    GLint nUniforms, size, location, maxLen;
    GLchar * name;
    GLsizei written;
    GLenum type;

    glGetProgramiv( m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen );
    glGetProgramiv( m_handle, GL_ACTIVE_UNIFORMS, &nUniforms );

    name = (GLchar *) malloc( maxLen );

    printf(" Location | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nUniforms; ++i ) {
        glGetActiveUniform( m_handle, i, maxLen, &written, &size, &type, name );
        location = glGetUniformLocation( m_handle, name );
        printf(" %-8d | %s\n",location, name );
    }

    free( name );
}

// **************************
//
void GLSLProgram::PrintActiveAttribs()
{
    
    GLint written, size, location, maxLength, nAttribs;
    GLenum type;
    GLchar * name;

    glGetProgramiv( m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength );
    glGetProgramiv( m_handle, GL_ACTIVE_ATTRIBUTES, &nAttribs );

    name = (GLchar *) malloc( maxLength );

    printf(" Index | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nAttribs; i++ ) {
        glGetActiveAttrib( m_handle, i, maxLength, &written, &size, &type, name );
        location = glGetAttribLocation( m_handle, name );
        printf(" %-5d | %s\n",location, name );
    }

    free( name );
}

// **************************
//
bool GLSLProgram::Validate()
{
    if( !IsLinked() )
    {
        return false;
    }

    GLint status;
    glValidateProgram( m_handle );
    glGetProgramiv( m_handle, GL_VALIDATE_STATUS, &status );

    if( GL_FALSE == status ) {
        // Store log and return false
        int length = 0;
        m_logString = "";

        glGetProgramiv( m_handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 )
        {
            char * c_log = new char[ length ];
            int written = 0;
            
            glGetProgramInfoLog( m_handle, length, &written, c_log );
            
            m_logString = c_log;
            
            delete [] c_log;
        }

        return false;
    }
    else
    {
       return true;
    }
}

// **************************
//
int GLSLProgram::GetUniformLocation( const char * name )
{
    return glGetUniformLocation( m_handle, name );
}

// **************************
//
bool GLSLProgram::FileExists( const std::string & fileName )
{
    struct stat info;
    int ret = -1;

    ret = stat(fileName.c_str(), &info);
    return 0 == ret;
}

} // bv
