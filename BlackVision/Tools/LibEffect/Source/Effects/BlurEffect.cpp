#include "BlurEffect.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"
#include "Engine/Models/Plugins/EngineConstantsMapper.h"
#include "Engine/Models/Builder/RendererStatesBuilder.h"
#include "Engine/Types/Values/ValuesFactory.h"

#include "UseLoggerLibEffect.h"

#include "IO/FileIO.h"
#include "System/Path.h"
#include <sstream>

namespace bv { namespace effect
{

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

    auto shader = new PixelShader( GetPSSource(), params );
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

    auto shader = new VertexShader( GetVSSource(), params );

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

// ****************************
//
std::string			BlurEffect::GetPSSource		() const
{
	std::string psFilename = "Assets/Shaders/Effects/blur.frag";

	return ReadFile( psFilename );
}

// ****************************
//
std::string			BlurEffect::GetVSSource		() const
{
	std::string vsFilename = "Assets/Shaders/Effects/blur.vert";

	return ReadFile( vsFilename );
}

// ****************************
//
std::string				BlurEffect::ReadFile		( const std::string & fileName )
{
	if( Path::Exists( fileName ) )
    {
        LOG_MESSAGE( SeverityLevel::debug ) << "BlurEffect: Loading pixel shader from: " << fileName;

		std::stringstream shaderSource;

		File::Open( fileName ) >> shaderSource;

		return shaderSource.str();

    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "BlurEffect: File " << fileName << " does not exist. Loading pixel shader failed.";
        assert( false );
        return "";
    }
}


} // effect
} // bv