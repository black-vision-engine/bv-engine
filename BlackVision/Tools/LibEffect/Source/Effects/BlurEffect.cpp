#include "BlurEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Models/Plugins/EngineConstantsMapper.h"

namespace bv { namespace effect
{

const std::string fragShaderSource = "					\
												\n		\
#version 400									\n		\
												\n		\
layout (location = 0) out vec4 FragColor;		\n		\
												\n		\
in vec2 uvCoord;								\n		\
												\n		\
uniform sampler2D Tex0;							\n		\
												\n		\
void main()										\n		\
{												\n		\
	vec4 col = texture( Tex0, uvCoord );		\n		\
	FragColor = col; 							\n		\
}												\n		\
";

const std::string vertShaderSource = "											 \
#version 400																\n	 \
																			\n	 \
layout (location = 0) in vec3 vertexPosition;								\n	 \
layout (location = 1) in vec2 vertexTexCoord;								\n	 \
																			\n	 \
uniform mat4 MVP;															\n	 \
uniform mat4 MV;															\n	 \
uniform mat4 P;																\n	 \
																			\n	 \
uniform mat4 txMat;															\n	 \
																			\n	 \
out vec2 uvCoord;															\n	 \
																			\n	 \
void main()																	\n	 \
{																			\n	 \
    gl_Position = MVP * vec4( vertexPosition, 1.0 );						\n	 \
    uvCoord = ( txMat * vec4( vertexTexCoord, 0.0, 1.0 ) ).xy;				\n	 \
}																				 \
";

// ****************************
//
BlurEffect::BlurEffect    ( Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor )
{
    auto ps = CreatePS( texture, filteringMode, wrapModeX, wrapModeY, borderColor );
    auto vs = CreateVS();

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );
    //auto sinst = pass->GetStateInstance();

    //RendererStatesBuilder::Create( sinst );

    //auto as = RenderStateAccessor::AccessAlphaState( sinst );
    //auto ds = RenderStateAccessor::AccessDepthState( sinst );
    //auto cs = RenderStateAccessor::AccessCullState( sinst );

    //as->blendEnabled = true;
    //ds->enabled = false;
    //cs->enabled = false;

    AddPass( pass );
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
    assert( texture != nullptr );

    auto params = new ShaderParameters();
    params->AddTexture( texture );

    auto shader = new PixelShader( fragShaderSource, params );
    auto sampler = CreateSampler( filteringMode, wrapModeX, wrapModeY, borderColor );

    shader->AddTextureSampler( sampler );

    return shader;
}

// ****************************
//
VertexShader *  BlurEffect::CreateVS   ()
{
    auto params     = new ShaderParameters();
    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();

    params->AddParameter( mvpParam );

    auto shader = new VertexShader( vertShaderSource, params );

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