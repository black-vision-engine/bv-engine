#include "BlurEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Models/Plugins/EngineConstantsMapper.h"

namespace bv { namespace effect
{

// ****************************
//
BlurEffect::BlurEffect    ()
{
	// TODO: Implement
    //auto ps = CreatePS( texture, filteringMode, wrapModeX, wrapModeY, borderColor, hasAlpha );
    //auto vs = CreateVS();

    //RenderablePass * pass = new RenderablePass( ps, vs, nullptr );
    //auto sinst = pass->GetStateInstance();

    //RendererStatesBuilder::Create( sinst );

    //auto as = RenderStateAccessor::AccessAlphaState( sinst );
    //auto ds = RenderStateAccessor::AccessDepthState( sinst );
    //auto cs = RenderStateAccessor::AccessCullState( sinst );

    //as->blendEnabled = true;
    //ds->enabled = false;
    //cs->enabled = false;

    //AddPass( pass );
}

// ****************************
//
BlurEffect::~BlurEffect    ()
{
    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
}

// ****************************
//
PixelShader *   BlurEffect::CreatePS   ( Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor )
{
	// TODO: Implement
    // FIXME: it may not be safe
    //assert( texture != nullptr );

    auto params = new ShaderParameters();
    params->AddTexture( texture );

    auto shader = new PixelShader( "", params );
    auto sampler = CreateSampler( filteringMode, wrapModeX, wrapModeY, borderColor );

    shader->AddTextureSampler( sampler );

    return shader;
}

// ****************************
//
VertexShader *  BlurEffect::CreateVS   ()
{
	// TODO: Implement
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto shader = new VertexShader( "", params );

    return shader;
}

// ****************************
//
TextureSampler *    BlurEffect::CreateSampler   ( TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor )
{
    auto wrapX          = EngineConstantsMapper::EngineConstant( wrapModeX );
    auto wrapY          = EngineConstantsMapper::EngineConstant( wrapModeY );            
    auto samplingMode   = SamplerSamplingMode::SSM_MODE_2D;
    auto sfm            = EngineConstantsMapper::EngineConstant( filteringMode );

    SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, SamplerWrappingMode::SWM_REPEAT };

    auto sampler = new TextureSampler( 0, "Tex0", samplingMode, sfm, wrappingMode, borderColor ); 

    return sampler;
}

} // effect
} // bv