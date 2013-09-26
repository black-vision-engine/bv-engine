#pragma once

#include "Engine\Models\Plugins\Interfaces\IGeometryShaderChannel.h"

namespace bv{ namespace model {

class GeometryShaderChannel : public IGeometryShaderChannel
{
    std::string             m_shaderFile;
public:

    virtual const std::string&      GetShaderFile() const override;

    explicit                        GeometryShaderChannel(const std::string& shaderFile);

    virtual                         ~GeometryShaderChannel() {}
};


} // model
} // bv