#pragma once

#include "Engine\Models\Plugins\Interfaces\IPixelShaderChannel.h"

namespace bv{ namespace model {

class PixelShaderChannel : public IPixelShaderChannel
{
    std::string             m_shaderFile;
public:

    virtual const std::string&      GetShaderFile() const override;

    explicit                        PixelShaderChannel(const std::string& shaderFile);

    virtual                         ~PixelShaderChannel() {}
};


} // model
} // bv