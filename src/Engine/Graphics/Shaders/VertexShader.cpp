#include "VertexShader.h"

namespace bv {

// *********************************
//
std::string PassThroughVertexShader::m_programSource = "   #version 400 \n \
                                                            \
                                                            layout (location = 0) in vec3 vertexPosition; \n \
                                                            \
                                                            uniform mat4 MVP; \n \
                                                            uniform mat4 MV; \n \
                                                            uniform mat4 P; \n \
                                                            mat4 dummy() \
                                                            {\
                                                                return MV * P * MVP;\
                                                            }\
                                                            \
                                                            void main() \n \
                                                            {\n \
                                                            dummy();\
                                                                gl_Position = MVP * vec4( vertexPosition, 1.0 );\n \
                                                            }\n \
                                                        ";


// *********************************
//
VertexShader::VertexShader( const std::string & programSource )
    : Shader( programSource )
{
    ShaderParameters* ret   = GetOrCreateShaderParameters();

    m_MVP                   = new ValueMat4("MVP");
    GenericShaderParam* genShaderParam = ShaderParamFactory::Get().Create( m_MVP->GetName(), m_MVP->GetParamType() );
    ret->RegisterParameter(genShaderParam);

    m_MV                    = new ValueMat4("MV");
    genShaderParam          = ShaderParamFactory::Get().Create( m_MV->GetName(), m_MV->GetParamType() );
    ret->RegisterParameter(genShaderParam);

    m_P                     = new ValueMat4("P");
    genShaderParam          = ShaderParamFactory::Get().Create( m_P->GetName(), m_P->GetParamType() );
    ret->RegisterParameter(genShaderParam);
}

// *********************************
//
void        VertexShader::UpdateMVP( const glm::mat4& MVP )
{
    GenericShaderParam* mvp = m_parameters->GetParam("MVP");
    assert(mvp);

    mvp->GenericSetVal(&MVP);
}

// *********************************
//
void        VertexShader::UpdateMV( const glm::mat4& MV )
{
    GenericShaderParam* mv = m_parameters->GetParam("MV");
    assert(mv);

    mv->GenericSetVal(&MV);
}

// *********************************
//
void        VertexShader::UpdateP( const glm::mat4& P )
{
    GenericShaderParam* p = m_parameters->GetParam("P");
    assert(p);

    p->GenericSetVal(&P);
}

// ***************************** DESCRIPTOR **********************************
//PLUGIN NAME
const std::string PassThroughVertexShaderParametersDescriptor::pluginName( "pastruvertexszader" );

//PLUGIN PARAMETERS
const std::string  PassThroughVertexShaderParametersDescriptor::MVPParamName( "MVP" );

// *********************************
//
PassThroughVertexShaderParametersDescriptor::PassThroughVertexShaderParametersDescriptor()
    : BaseParametersDescriptor( "passthroughvertexshader" )
{
    m_params[ MVPParamName ]    = ParamType::PT_MAT4;
}

// *********************************
//
PassThroughVertexShader::PassThroughVertexShader(PassThroughVertexShaderParametersDescriptor* desc)
    : VertexShader( PassThroughVertexShader::m_programSource )
{
    //FIXME: pass params in constructor
    desc->ValidateParameters(PassThroughVertexShader::m_programSource);
}



} //bv

//#include <fstream>
//using std::ifstream;
//using std::ios;
//
//#include <sstream>
//using std::ostringstream;
//
//#include <sys/stat.h>
//
//namespace bv {
//
//VertexShader::VertexShader(const std::string& programName, int inputNum, int outputNum)
//    : Shader(programName, inputNum, outputNum)
//{}
//
//
//VertexShader* VertexShader::create(const std::string& programFile, const std::string& programName, int inputNum, int outputNum)
//{
//    if( ! fileExists(programFile) )
//    {
//        return nullptr;
//    }
//
//    ifstream inFile( programFile, ios::in );
//    if( !inFile ) {
//        return false;
//    }
//
//    ostringstream code;
//    while( inFile.good() )
//    {
//        int c = inFile.get();
//        if( ! inFile.eof() )
//            code << (char) c;
//    }
//
//    inFile.close();
//
//    VertexShader* ret = new VertexShader(programName, inputNum, outputNum);
//
//    ret->programSource = code.str();
//
//    return ret;
//}
//
//} // bv