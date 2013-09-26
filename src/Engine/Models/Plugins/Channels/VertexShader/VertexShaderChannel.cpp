#include "VertexShaderChannel.h"

namespace bv{ namespace model {

VertexShaderChannel::VertexShaderChannel(const std::string& shaderFile)
    : m_shaderFile( shaderFile  )
{}


const std::string&      VertexShaderChannel::GetShaderFile() const
{
    return m_shaderFile;
}

} // model
} // bv