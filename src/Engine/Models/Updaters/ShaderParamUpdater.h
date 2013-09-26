#pragma once

#include "IUpdater.h"

namespace bv
{

namespace model
{
    class IShaderChannel;
}

class Shader;

class ShaderParamUpdater : public IUpdater
{
private:

    const model::IShaderChannel *   m_shaderChannel;
    Shader *                        m_shader;

private:

                                    ShaderParamUpdater  ( const model::IShaderChannel *, Shader * );

public:

    virtual void                    Update              ( float t );
    static ShaderParamUpdater *     Create              ( const model::IShaderChannel *, Shader * );

};

}