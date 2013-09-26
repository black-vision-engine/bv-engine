#include "PixelShaderChannel.h"

namespace bv{ namespace model {

PixelShaderChannel::PixelShaderChannel(const std::string& shaderFile)
    : m_shaderFile( shaderFile  )
{}


const std::string&      PixelShaderChannel::GetShaderFile() const
{
    return m_shaderFile;
}

} // model
} // bv