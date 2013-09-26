#include "ShaderParamUpdater.h"
#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"
#include "Engine/Graphics/Shaders/Shader.h"

namespace bv
{

// ******************************
//
ShaderParamUpdater::ShaderParamUpdater( IShaderChannel * shCh, Shader * sh )
    : m_shaderChannel( shCh )
    , m_shader( sh )
{
}

// ******************************
//
void                        ShaderParamUpdater::Update( float t )
{
    assert( !"A co tutaj?" );
    // TODO
    //auto shaderParams = m_shader->Parameters();

    //for( auto val : m_shaderChannel->GetValuesList())
    //{
    //    shaderParams->GetParam(val->GetName())->SetValue(val->GetData());
    //}
}

// ******************************
//
ShaderParamUpdater *         ShaderParamUpdater::Create( IShaderChannel * shCh, Shader * sh )
{
    return new ShaderParamUpdater( shCh, sh );
}

}
