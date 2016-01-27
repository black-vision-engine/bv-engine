#include "FullscreenRenderableEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"

#include "Engine/Graphics/Effects/FullScreen/FullscreenVSShader.h"


namespace bv {

// **************************
//
FullscreenRenderableEffect::FullscreenRenderableEffect  ( RenderablePass * pass )
{
    AddPass( pass );
}

// **************************
//
VertexShader *  FullscreenRenderableEffect::CreateVS      ( unsigned int numUVChannels )
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
std::string     FullscreenRenderableEffect::GetVSShaderSource ( unsigned int numUVChannels )
{
    return FullscreenVSShader::GenerateDefaultVS( numUVChannels );
}

} //bv
