#include "DefaultVertexShaderChannel.h"

//FIXME: REMOVE after reimplementing access to values from shader channel implementation
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv { namespace model {

// ******************************
//
DefaultVertexShaderChannel::DefaultVertexShaderChannel  ( const std::string & shaderSource, const IValueSet * values )
    : ShaderChannel( shaderSource, values )
{
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
    return new DefaultVertexShaderChannel( shaderSource, new DefaultParamValModel() ); //FIXME: remove this DefaultParamValModel construction from here (implement decent ShaderChannel in case of nullptr input IValueSet - simply return empty vector there
}

} //model
} //bv
