#include "DefaultEffect.h"

#include <cassert>

#include "Engine/Interfaces/IShaderDataSource.h"
#include "Engine/Interfaces/IMultipassShaderDataSource.h"

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Models/Interfaces/IAnimationDescriptor.h"

#include "Engine/Models/Plugins/EngineConstantsMapper.h"

#include "Engine/Graphics/Resources/Textures/Texture2D.h"
#include "Engine/Graphics/Resources/Textures/Texture2DCache.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParameters.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"


namespace bv {

// *********************************
//
DefaultEffect::DefaultEffect     ( const IMultipassShaderDataSource * msds )
{
    for( unsigned int i = 0; i < msds->GetNumPasses(); ++i )
    {
        auto psds = msds->GetPixelShaderDataSource( i );
        auto vsds = msds->GetVertexShaderDataSource( i );
        auto gsds = msds->GetGeometryShaderDataSource( i );

        auto pass = CreateRenderablePass( psds, vsds, gsds );

        AddPass( pass );
    }
}

// *********************************
//
DefaultEffect::DefaultEffect    ( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds )
{
    auto pass = CreateRenderablePass( psds, vsds, gsds );

    AddPass( pass );
}

// *********************************
//
DefaultEffect::~DefaultEffect   ()
{
    //FIXME: this suxx as this class cleans up shaders from passes and passes itself are deleted in the base class (delete pass should delete shaders as well) - to be fixed
    for( unsigned int i = 0; i < NumPasses(); ++i )
    {
        auto pass = GetPass( i );

        delete pass->GetPixelShader();
        delete pass->GetVertexShader();
        delete pass->GetGeometryShader();
    }
}

// *********************************
//
RenderablePass *    DefaultEffect::CreateRenderablePass    ( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds ) const
{
    assert( psds != nullptr );
    assert( vsds != nullptr );

    ShaderParameters * psparams = CreateDefaultParamsPS( psds );
    ShaderParameters * vsparams = CreateDefaultParamsVS( vsds );

    assert( psparams != nullptr );
    assert( vsparams != nullptr );

    PixelShader * ps    = new PixelShader( psds->GetShaderSource(), psparams );
    VertexShader * vs   = new VertexShader( vsds->GetShaderSource(), vsparams );

    AddTextures( ps, psds->GetTexturesData() );
    AddTextures( vs, vsds->GetTexturesData() );

    GeometryShader * gs = nullptr;
    if ( gsds != nullptr )
    {
        ShaderParameters * gsparams = CreateDefaultParamsGS( gsds );

        assert( gsparams != nullptr );

        gs = new GeometryShader( gsds->GetShaderSource(), gsparams );

        AddTextures( vs, gsds->GetTexturesData() );
    }

    RenderablePass * pass = new RenderablePass( ps, vs, gs );

    return pass;
}

// *********************************
//
ShaderParameters *  DefaultEffect::CreateDefaultParamsPS  ( const IShaderDataSource * ds ) const
{
    auto sp = CreateDefaultParamsImpl( ds );

    return sp;
}

// *********************************
//
ShaderParameters * DefaultEffect::CreateDefaultParamsVS  ( const IShaderDataSource * ds ) const
{
    auto params = CreateDefaultParamsImpl( ds );

    assert( params != nullptr );

    auto mvpParam   = ShaderParamFactory::CreateMVPParameter();
    auto mvParam    = ShaderParamFactory::CreateMVParameter ();
    auto pParam     = ShaderParamFactory::CreatePParameter  ();

    params->AddParameter( mvpParam );
    params->AddParameter( mvParam );
    params->AddParameter( pParam );

    return params;
}

// *********************************
//
ShaderParameters * DefaultEffect::CreateDefaultParamsGS  ( const IShaderDataSource * ds ) const
{
    if( ds )
    {
        auto gs = CreateDefaultParamsVS( ds );

        return gs;
    }

    return nullptr;
}

// *********************************
// FIXME: only 2D textures right now
void               DefaultEffect::AddTextures       ( Shader * shader, ITexturesDataConstPtr txData ) const
{
    unsigned int samplerNum = 0;

    if ( shader && txData )
    {
        auto params     = shader->GetParameters ();
        auto textures   = txData->GetTextures   ();
        auto animations = txData->GetAnimations ();

        for( auto tx : textures )
        {
            auto sampler = CreateSampler( tx, samplerNum );
            auto texture = GetTexture( tx );

            shader->AddTextureSampler( sampler );
            params->AddTexture( texture );

            samplerNum++;
        }

        for( auto anim : animations )
        {
            if( anim->NumTextures() > 0 )
            {
                auto sampler    = CreateSampler( anim, samplerNum );
                auto sequence   = GetSequence( anim );

                shader->AddTextureSampler( sampler );
                params->AddTexture( sequence );

                samplerNum++;
            }
        }
    }
}

// *********************************
//
ShaderParameters *      DefaultEffect::CreateDefaultParamsImpl ( const IShaderDataSource * ds ) const
{
    ShaderParameters * sp = nullptr;

    if( ds != nullptr )
    {
        sp = new ShaderParameters();

        for( auto value : ds->GetValues() )
        {
            GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( value.get() );
            assert( param != nullptr );

            sp->AddParameter( param );
        }
    }

    return sp;
}

// *********************************
//
TextureSampler *        DefaultEffect::CreateSampler   ( const ITextureParams * txParams, unsigned int samplerNum ) const
{
    auto wrapX          = EngineConstantsMapper::EngineConstant( txParams->GetWrappingModeX() );
    auto wrapY          = EngineConstantsMapper::EngineConstant( txParams->GetWrappingModeY() );
	auto wrapZ          = EngineConstantsMapper::EngineConstant( txParams->GetWrappingModeZ() );

	auto w = txParams->GetWidth();
	auto h = txParams->GetHeight();
	auto d = txParams->GetDepth();

	assert( w > 0 && h > 0 && d > 0 );

    SamplerSamplingMode samplingMode = SamplerSamplingMode::ST_TOTAL;

	if( w >= 1 && h == 1 && d == 1 )
	{
		samplingMode = SamplerSamplingMode::SSM_MODE_1D;
	}

	if( w >= 1 && h > 1 && d == 1 )
	{
		samplingMode = SamplerSamplingMode::SSM_MODE_2D;
	}

	if( w >= 1 && h > 1 && d > 1 )
	{
		samplingMode = SamplerSamplingMode::SSM_MODE_3D;
	}

    auto filteringMode  = EngineConstantsMapper::EngineConstant( txParams->GetFilteringMode() );
    auto borderColor    = txParams->BorderColor();

    SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, wrapZ };

    auto sampler = new TextureSampler( samplerNum, txParams->GetName(), samplingMode, filteringMode, wrappingMode, borderColor ); 

    return sampler;
}

// *********************************
//
Texture2DPtr            DefaultEffect::GetTexture           ( const ITextureDescriptor * txParams ) const
{
    return GTexture2DCache.GetTexture( txParams );
}

// *********************************
//
Texture2DPtr            DefaultEffect::GetSequence          ( const IAnimationDescriptor * animParams ) const
{
    return GTexture2DCache.GetSequence( animParams );
}

} //bv
