#include "DefaultVertexShaderChannel.h"

//FIXME: REMOVE after reimplementing access to values from shader channel implementation
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model {

// ******************************
//
DefaultVertexShaderChannel::DefaultVertexShaderChannel  ( const std::string & shaderSource, const IValueSet * values )
    : ShaderChannel( shaderSource, values )
    , m_texturesData( nullptr )
{
    m_texturesData = new DefaultTexturesData();
}

// ******************************
//
DefaultVertexShaderChannel::~DefaultVertexShaderChannel                     ()
{
    delete m_texturesData;
}

// ******************************
//
const ITexturesData *       DefaultVertexShaderChannel::GetTexturesData     () const
{
    return m_texturesData;
}

// ******************************
//
DefaultTexturesData *       DefaultVertexShaderChannel::GetTexturesDataImpl ()
{
    return m_texturesData;
}

// ******************************
//
DefaultVertexShaderChannel * DefaultVertexShaderChannel::Create             ( const std::string & shaderFile, const IValueSet * values )
{
    auto shaderSource = ReadShaderFromFile( shaderFile );

    if( shaderSource != "" )
    {
        assert( values );
        
        return new DefaultVertexShaderChannel( shaderSource, values );
    }

    return nullptr;
}

namespace {

// *********************************
//
std::string shaderSource   = " #version 400 \n \
                                \
                                layout (location = 0) in vec3 vertexPosition; \n \
                                \
                                uniform mat4 MVP; \n \
                                \
                                void main() \n \
                                {\n \
                                    gl_Position = MVP * vec4( vertexPosition, 1.0 );\n \
                                }\n \
                            ";

}

// ******************************
//
DefaultVertexShaderChannel * DefaultVertexShaderChannel::Create ()
{
    //FIXME: remove this DefaultParamValModel construction from here (implement decent ShaderChannel in case of nullptr input IValueSet - simply return empty vector there)
    return new DefaultVertexShaderChannel( shaderSource, new DefaultParamValModel() );
}

} //model
} //bv
