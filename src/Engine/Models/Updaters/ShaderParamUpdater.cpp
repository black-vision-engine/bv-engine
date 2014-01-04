#include "ShaderParamUpdater.h"

#include "Engine/Models/Plugins/Interfaces/IShaderChannel.h"
#include "Engine/Graphics/Shaders/Shader.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"


namespace bv
{

// ******************************
//
ShaderParamUpdater::ShaderParamUpdater( const model::IShaderChannel * shCh, Shader * sh )
    : m_shaderChannel( shCh )
    , m_shader( sh )
{
}

// ******************************
//
void                        ShaderParamUpdater::DoUpdate( TimeType t )
{
    auto shaderParams = m_shader->Parameters();

    for( auto val : m_shaderChannel->GetValuesList() )
    {
        shaderParams->GetParam( val->GetName() )->SetValue( val->GetData() );
    }
}

// ******************************
//
ShaderParamUpdater *         ShaderParamUpdater::Create( const model::IShaderChannel * shCh, Shader * sh )
{
    return new ShaderParamUpdater( shCh, sh );
}

}
