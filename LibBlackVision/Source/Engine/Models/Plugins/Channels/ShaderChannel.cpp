#include "ShaderChannel.h"

namespace bv { namespace model {

// *********************************
//
const std::string                            ReadShaderContentsFromFile             ( const std::string & fileName )
{
    std::stringstream shaderSource;

    File::Open( fileName ) >> shaderSource;

    return shaderSource.str();
}

} // model
} // bv
