#pragma once

#include "IUpdater.h"

namespace bv
{

class IShaderPlugin;
class Shader;

class ShaderParamUpdater : public IUpdater
{
private:

    IShaderPlugin *     m_shaderPlugin;
    Shader *            m_shader;

private:

                                    ShaderParamUpdater  ( IShaderPlugin *, Shader * );

public:

    virtual void                    Update              ( float t );
    static ShaderParamUpdater *     Create              ( IShaderPlugin *, Shader * );

};

}