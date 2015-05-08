#include "BlurEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Models/Plugins/EngineConstantsMapper.h"
#include "Engine/Models/Builder/RendererStatesBuilder.h"
#include "Engine/Types/Values/ValuesFactory.h"

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
uniform sampler2D	Tex0;						\n		\
uniform int			blurLenght;					\n		\
uniform float		pixelWidth;					\n		\
uniform float		pixelHeight;				\n		\
												\n		\
void main()										\n		\
{												\n		\
	vec4 sum = vec4( 0.0, 0.0, 0.0, 0.0 );					\n \
	for( int i = -blurLenght; i <= blurLenght; ++i )		\n \
		for( int j = -blurLenght; j <= blurLenght; ++j )		\n \
			sum += texture( Tex0, uvCoord + vec2( i * pixelWidth, j * pixelHeight ) );	\n \
	FragColor = sum / ( ( 2 * blurLenght + 1 ) * ( 2 * blurLenght + 1 ) );				\n \
}															\n \
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
    uvCoord = vertexTexCoord;												\n	 \
}																				 \
";

// ****************************
//
BlurEffect::BlurEffect    ( UInt32 blurLenght, Float32 pixelWidth, Float32 pixelHeight, Texture2DPtr texture, TextureFilteringMode filteringMode, TextureWrappingMode wrapModeX, TextureWrappingMode wrapModeY, const glm::vec4 & borderColor )
	: m_blurLength( blurLenght )
	, m_pixelWidth( pixelWidth )
	, m_pixelHeight( pixelHeight )
{
    auto ps = CreatePS( texture, filteringMode, wrapModeX, wrapModeY, borderColor );
    auto vs = CreateVS();

    RenderablePass * pass = new RenderablePass( ps, vs, nullptr );
    auto sinst = pass->GetStateInstance();

    RendererStatesBuilder::Create( sinst );

    auto as = RenderStateAccessor::AccessAlphaState( sinst );
    auto ds = RenderStateAccessor::AccessDepthState( sinst );
    auto cs = RenderStateAccessor::AccessCullState( sinst );

    as->blendEnabled = false;
    ds->enabled = false;
    cs->enabled = false;

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

	m_blurLenghtVal = ValuesFactory::CreateValueInt( "blurLenght" );
	m_blurLenghtVal->SetValue( m_blurLength );

	params->AddParameter( ShaderParamFactory::CreateGenericParameter( m_blurLenghtVal.get() ) );

	m_pixelWidthVal = ValuesFactory::CreateValueFloat( "pixelWidth" );
	m_pixelWidthVal->SetValue( m_pixelWidth );

	params->AddParameter( ShaderParamFactory::CreateGenericParameter( m_pixelWidthVal.get() ) );

	m_pixelHeightVal = ValuesFactory::CreateValueFloat( "pixelHeight" );
	m_pixelHeightVal->SetValue( m_pixelHeight );

	params->AddParameter( ShaderParamFactory::CreateGenericParameter( m_pixelHeightVal.get() ) );


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