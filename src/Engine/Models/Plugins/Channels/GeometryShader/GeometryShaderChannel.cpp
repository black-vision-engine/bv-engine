#include "GeometryShaderChannel.h"

namespace bv{ namespace model {

GeometryShaderChannel::GeometryShaderChannel(const std::string& shaderFile)
    : m_shaderFile( shaderFile  )
{}


const std::string&      GeometryShaderChannel::GetShaderFile() const
{
    return m_shaderFile;
}

} // model
} // bv