#include "ShaderParamUpdater.h"
#include "Engine/Models/IPlugin.h"
#include "Engine/Graphics/Shaders/Shader.h"

namespace bv
{


// ******************************
//
ShaderParamUpdater::ShaderParamUpdater( IShaderPlugin * pl, Shader * sh )
    : m_shaderPlugin( pl )
    , m_shader( sh )
{
}

// ******************************
//
void                        ShaderParamUpdater::Update( float t )
{
    auto shaderParams = m_shader->Parameters();

    for( auto val : m_shaderPlugin->GetValuesList())
    {
        shaderParams->GetParam(val->GetName())->SetValue(val->GetData());
    }
}

// ******************************
//
ShaderParamUpdater *         ShaderParamUpdater::Create( IShaderPlugin * pl, Shader * sh )
{
    return new ShaderParamUpdater( pl, sh );
}

}
