#pragma once

#include <vector>
#include "glm/glm.hpp"

#include "Engine\Models\Plugin.h"

namespace bv {

class ValueMat4;

class PluginTransform : public BasePlugin<ITransformPlugin>
{
protected:
    ValueMat4*              m_value;

public:
    const glm::mat4 &           TransformMatrix     ()                                  const;
    virtual void                Print               (std::ostream& out, int tabs = 0)   const;

    explicit PluginTransform(const std::string& name);
    virtual ~PluginTransform(){};
};

}
