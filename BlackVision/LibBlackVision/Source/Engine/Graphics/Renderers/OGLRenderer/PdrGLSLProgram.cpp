#include "PdrGLSLProgram.h"

#include <sys/stat.h>

#include <fstream>
#include <sstream>

#include "Engine/Graphics/Renderers/OGLRenderer/glutils.h"

#include "Engine/Graphics/Shaders/PixelShader.h"
#include "Engine/Graphics/Shaders/VertexShader.h"
#include "Engine/Graphics/Shaders/GeometryShader.h"


using std::ifstream;
using std::ios;
using std::ostringstream;

const bool GVerboseLogging = false;


namespace bv {

// *******************************
//
PdrGLSLProgram::PdrGLSLProgram() 
    : m_Handle( 0 )
    , m_Linked( false )
    , m_Compiled( false )
    , m_verboseLogging( GVerboseLogging )
{ 
}

bool compileAndLinkProgram  ( PdrGLSLProgram & prog, const std::string & vs, const std::string & ps, const std::string & gs )
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
PdrGLSLProgram::PdrGLSLProgram( const PixelShader & ps, const VertexShader & vs, const GeometryShader * gs )
    : m_Handle( 0 )
    , m_Linked( false ) 
{
    m_Compiled = compileAndLinkProgram( *this, vs.ProgramSource(), ps.ProgramSource(), gs ? gs->ProgramSource() : "" );
    assert( m_Compiled ); //FIXME: this error should be handled in a somehow different manner
}

// *******************************
//
PdrGLSLProgram::~PdrGLSLProgram					    ()
{
    glDeleteProgram( m_Handle );
}

// *******************************
//
bool PdrGLSLProgram::CompileShaderFromFile( const string & fileName, GLSLShader::GLSLShaderType type )
{
    if( !FileExists( fileName ) )
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
bool PdrGLSLProgram::CompileShaderFromString( const string & source, GLSLShader::GLSLShaderType type )
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
bool PdrGLSLProgram::Link()
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
void PdrGLSLProgram::Use()
{
    if( m_Handle <= 0 || (!m_Linked) )
        return;

    glUseProgram( m_Handle );
}

// *******************************
//
string PdrGLSLProgram::Log() const
{
    return m_LogString;
}

// *******************************
//
int PdrGLSLProgram::GetHandle() const
{
    return m_Handle;
}

// *******************************
//
bool PdrGLSLProgram::IsLinked() const
{
    return m_Linked;
}

// *******************************
//
bool PdrGLSLProgram::IsCompiled() const
{
    return m_Compiled;
}

// *******************************
//
void PdrGLSLProgram::BindAttribLocation( GLuint location, const string & name )
{
    glBindAttribLocation(m_Handle, location, name.c_str() );
}

// *******************************
//
void PdrGLSLProgram::BindFragDataLocation( GLuint location, const string & name )
{
    glBindFragDataLocation(m_Handle, location, name.c_str() );
}

// *******************************
//
void          PdrGLSLProgram::PostSetUniformFail    ( const string & name )
{
    if ( m_verboseLogging )
        printf( "Uniform: %s not found.\n", name.c_str() );
}

// *******************************
//
void          PdrGLSLProgram::PostSetUniformFail    ( int loc )
{
    if ( m_verboseLogging )
        printf("Uniform at loc: %d not found.\n", loc );
}

// *******************************
//
void PdrGLSLProgram::PrintActiveUniforms()
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
void PdrGLSLProgram::PrintActiveAttribs()
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
bool PdrGLSLProgram::Validate()
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
bool PdrGLSLProgram::FileExists( const string & fileName ) const
{
    struct stat info;
    int ret = -1;

    ret = stat( fileName.c_str(), &info );

    return 0 == ret;
}

//********************** GLSLPrograms ******************//

// *******************************
//
PdrGLSLProgram *  PdrGLSLPrograms::getProgram( PixelShader * ps, VertexShader * vs )
{
    assert( ps && vs );
    auto it = programs.find(std::make_pair(ps,vs));

    if(it != programs.end())
        return it->second;
    else
    {
        auto ret = new PdrGLSLProgram( *ps,*vs, nullptr );
        programs[std::make_pair(ps,vs)] = ret;
        return ret;
    }
}

// *******************************
//
PdrGLSLPrograms & PdrGLSLPrograms::get()
{
    //static PdrGLSLPrograms & instance = PdrGLSLPrograms();
    static PdrGLSLPrograms instance = PdrGLSLPrograms();

    return instance;
}

} //bv
