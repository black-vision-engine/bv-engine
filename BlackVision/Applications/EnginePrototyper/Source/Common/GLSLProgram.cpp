#include "glslprogram.h"

#include <fstream>
#include <sstream>

#include <sys/stat.h>

#include "UseLoggerLibBlackVision.h"

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
    BVGL::bvglDeleteProgram( m_handle );    
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
        m_handle = BVGL::bvglCreateProgram();
        
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
        m_handle = BVGL::bvglCreateProgram();
        
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
            shaderHandle = BVGL::bvglCreateShader( GL_VERTEX_SHADER );
            break;
        case GL_FRAGMENT_SHADER:
            shaderHandle = BVGL::bvglCreateShader( GL_FRAGMENT_SHADER );
            break;
        case GL_GEOMETRY_SHADER:
            shaderHandle = BVGL::bvglCreateShader( GL_GEOMETRY_SHADER );
            break;
        case GL_TESS_CONTROL_SHADER:
            shaderHandle = BVGL::bvglCreateShader( GL_TESS_CONTROL_SHADER );
            break;
        case GL_TESS_EVALUATION_SHADER:
            shaderHandle = BVGL::bvglCreateShader( GL_TESS_EVALUATION_SHADER );
            break;
        default:
            return false;
    }

    const char * c_code = source.c_str();
   
    BVGL::bvglShaderSource( shaderHandle, 1, &c_code, NULL );

    // Compile the shader
    BVGL::bvglCompileShader( shaderHandle );

    // Check for errors
    int result;

    BVGL::bvglGetShaderiv( shaderHandle, GL_COMPILE_STATUS, &result );
    
    if( GL_FALSE == result )
    {
        // Compile failed, store log and return false
        int length = 0;

        m_logString = "";
        
        BVGL::bvglGetShaderiv( shaderHandle, GL_INFO_LOG_LENGTH, &length );
        
        if( length > 0 )
        {
            char * c_log = new char[ length ];
            int written = 0;
            BVGL::bvglGetShaderInfoLog(shaderHandle, length, &written, c_log);
            
            m_logString = c_log;
            
            delete [] c_log;
        }

        return false;
    } 
    else 
    {
        // Compile succeeded, attach shader and return true
        BVGL::bvglAttachShader( m_handle, shaderHandle );

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

    BVGL::bvglLinkProgram( m_handle );

    int status = 0;

    BVGL::bvglGetProgramiv( m_handle, GL_LINK_STATUS, &status );

    if( GL_FALSE == status )
    {
        // Store log and return false
        int length = 0;
        
        m_logString = "";

        BVGL::bvglGetProgramiv( m_handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 )
        {
            char * c_log = new char[ length ];
        
            int written = 0;
            
            BVGL::bvglGetProgramInfoLog( m_handle, length, &written, c_log );
            
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

    BVGL::bvglUseProgram( m_handle );
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
    BVGL::bvglBindAttribLocation( m_handle, location, name );
}

// **************************
//
void GLSLProgram::BindFragDataLocation( GLuint location, const char * name )
{
    BVGL::bvglBindFragDataLocation( m_handle, location, name);
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, float x, float y, float z )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        BVGL::bvglUniform3f( loc, x, y, z );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Uniform: '" << name << "' not found.";
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
        BVGL::bvglUniform4f( loc, v.x, v.y, v.z, v.w );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Uniform: '" << name << "' not found.";
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, const glm::vec2 & v )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        BVGL::bvglUniform2f( loc, v.x, v.y );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Uniform: '" << name << "' not found.";
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, const glm::mat4 & m )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        BVGL::bvglUniformMatrix4fv( loc, 1, GL_FALSE, &m[0][0] );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Uniform: '" << name << "' not found.";
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, const glm::mat3 & m )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        BVGL::bvglUniformMatrix3fv( loc, 1, GL_FALSE, &m[0][0] );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Uniform: '" << name << "' not found.";
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, float val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        BVGL::bvglUniform1f( loc, val );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Uniform: '" << name << "' not found.";
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, int val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        BVGL::bvglUniform1i( loc, val );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Uniform: '" << name << "' not found.";
    }
}

// **************************
//
void GLSLProgram::SetUniform( const char * name, bool val )
{
    int loc = GetUniformLocation( name );

    if( loc >= 0 )
    {
        BVGL::bvglUniform1i( loc, val );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Uniform: '" << name << "' not found.";
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

    BVGL::bvglGetProgramiv( m_handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLen );
    BVGL::bvglGetProgramiv( m_handle, GL_ACTIVE_UNIFORMS, &nUniforms );

    name = (GLchar *) malloc( maxLen );

    printf(" Location | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nUniforms; ++i ) {
        BVGL::bvglGetActiveUniform( m_handle, i, maxLen, &written, &size, &type, name );
        location = BVGL::bvglGetUniformLocation( m_handle, name );
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

    BVGL::bvglGetProgramiv( m_handle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength );
    BVGL::bvglGetProgramiv( m_handle, GL_ACTIVE_ATTRIBUTES, &nAttribs );

    name = (GLchar *) malloc( maxLength );

    printf(" Index | Name\n");
    printf("------------------------------------------------\n");
    for( int i = 0; i < nAttribs; i++ ) {
        BVGL::bvglGetActiveAttrib( m_handle, i, maxLength, &written, &size, &type, name );
        location = BVGL::bvglGetAttribLocation( m_handle, name );
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
    BVGL::bvglValidateProgram( m_handle );
    BVGL::bvglGetProgramiv( m_handle, GL_VALIDATE_STATUS, &status );

    if( GL_FALSE == status ) {
        // Store log and return false
        int length = 0;
        m_logString = "";

        BVGL::bvglGetProgramiv( m_handle, GL_INFO_LOG_LENGTH, &length );

        if( length > 0 )
        {
            char * c_log = new char[ length ];
            int written = 0;
            
            BVGL::bvglGetProgramInfoLog( m_handle, length, &written, c_log );
            
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
    return BVGL::bvglGetUniformLocation( m_handle, name );
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
