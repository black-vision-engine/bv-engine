#include "DefaultVertexShaderChannel.h"


namespace {
//Pass through shader
std::string GPassThroughProgramSource = "   #version 400 \n \
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


} //anonymous

namespace bv { namespace model {

// ******************************
//
DefaultVertexShaderChannel::DefaultVertexShaderChannel()
    : ShaderChannel( GPassThroughProgramSource, nullptr )
{
}

} // model
} // bv
