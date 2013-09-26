#pragma once

#include "IUpdater.h"

namespace bv
{

class IShaderChannel;
class Shader;

class ShaderParamUpdater : public IUpdater
{
private:

    IShaderChannel *    m_shaderChannel;
    Shader *            m_shader;

private:

                                    ShaderParamUpdater  ( IShaderChannel *, Shader * );

public:

    virtual void                    Update              ( float t );
    static ShaderParamUpdater *     Create              ( IShaderChannel *, Shader * );

};

}