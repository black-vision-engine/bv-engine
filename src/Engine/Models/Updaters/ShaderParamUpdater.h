#pragma once

#include "Engine/Interfaces/IUpdater.h"


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

    model::IShaderChannel *   m_shaderChannel;
    Shader *                        m_shader;

private:

                                    ShaderParamUpdater  ( model::IShaderChannel *, Shader * );

public:

    virtual void                    DoUpdate            ( TimeType t );

    static ShaderParamUpdater *     Create              ( model::IShaderChannel *, Shader * );

};

} //bv
