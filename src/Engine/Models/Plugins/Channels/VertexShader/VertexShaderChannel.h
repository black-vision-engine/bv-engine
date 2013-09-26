#pragma once

#include "Engine\Models\Plugins\Interfaces\IVertexShaderChannel.h"

namespace bv{ namespace model {

class VertexShaderChannel : public IVertexShaderChannel
{
    std::string             m_shaderFile;
public:

    virtual const std::string&      GetShaderFile() const override;

    explicit                        VertexShaderChannel(const std::string& shaderFile);

    virtual                         ~VertexShaderChannel() {}
};


} // model
} // bv