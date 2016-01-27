#include "FullscreenRenderableEffectOld.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Effects/FullScreen/FullscreenVSShader.h"


namespace bv {

// **************************
//
FullscreenRenderableEffectOld::FullscreenRenderableEffectOld  ( RenderablePass * pass )
{
    AddPass( pass );
}

// **************************
//
VertexShader *  FullscreenRenderableEffectOld::CreateVS      ( unsigned int numUVChannels )
{
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto shader = new VertexShader( GetVSShaderSource( numUVChannels ), params );

    //FIXME: add empty textures (nullptr) and create samplers. Textures can be set later on
    return shader;
}

// **************************
//
std::string     FullscreenRenderableEffectOld::GetVSShaderSource ( unsigned int numUVChannels )
{
    return FullscreenVSShader::GenerateDefaultVS( numUVChannels );
}

} //bv
