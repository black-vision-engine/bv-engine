#include "stdafx.h"

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




#include "Memory/MemoryLeaks.h"



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
{}

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
    auto mParam     = ShaderParamFactory::CreateMParameter  ();
    auto vParam     = ShaderParamFactory::CreateVParameter  ();
    //auto pParam     = ShaderParamFactory::CreatePParameter  ();       // This parameter was never used, but you can uncommment it in future.
    auto normParamMV= ShaderParamFactory::CreateNormalMatrixMVParameter  ();
    auto normParamM = ShaderParamFactory::CreateNormalMatrixMParameter  ();

    params->AddParameter( mvpParam );
    params->AddParameter( mvParam );
    params->AddParameter( mParam );
    params->AddParameter( vParam );
    //params->AddParameter( pParam );
    params->AddParameter( normParamMV );
    params->AddParameter( normParamM );


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
            params->AddTexture( texture, CreateSamplerParameters( tx ) );

            samplerNum++;
        }

        for( auto anim : animations )
        {
            if( anim->NumTextures() > 0 )
            {
                auto sampler    = CreateSampler( anim, samplerNum );
                auto sequence   = GetSequence( anim );

                shader->AddTextureSampler( sampler );
                params->AddTexture( sequence, CreateSamplerParameters( anim ) );

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
TextureSampler *        DefaultEffect::CreateSampler   ( const ITextureParamsPtr & txParams, unsigned int samplerNum ) const
{
 //   auto wrapX          = EngineConstantsMapper::EngineConstant( txParams->GetWrappingModeX() );
 //   auto wrapY          = EngineConstantsMapper::EngineConstant( txParams->GetWrappingModeY() );
    //auto wrapZ          = EngineConstantsMapper::EngineConstant( txParams->GetWrappingModeZ() );

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

    //auto filteringMode  = EngineConstantsMapper::EngineConstant( txParams->GetFilteringMode() );
    //auto borderColor    = txParams->BorderColor();

    //SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, wrapZ };

    auto sampler = new TextureSampler( samplerNum, txParams->GetName(), samplingMode ); 

    return sampler;
}

// *********************************
//
Texture2DPtr            DefaultEffect::GetTexture           ( const ITextureDescriptorPtr & txParams ) const
{
    return GTexture2DCache.GetTexture( txParams.get() );
}

// *********************************
//
Texture2DPtr            DefaultEffect::GetSequence          ( const IAnimationDescriptorPtr & animParams ) const
{
    return GTexture2DCache.GetSequence( animParams.get() );
}

// *********************************
//
SamplerShaderParametersPtr	DefaultEffect::CreateSamplerParameters  ( const ITextureParamsPtr & txParams ) const
{
    auto samplerState = txParams->GetSamplerState();
    return std::make_shared< SamplerShaderParameters >( ( SamplerWrappingMode )samplerState->GetWrappingModeX(), ( SamplerWrappingMode )samplerState->GetWrappingModeY(), 
                        ( SamplerWrappingMode )samplerState->GetWrappingModeZ(), ( SamplerFilteringMode )samplerState->GetFilteringMode(), samplerState->GetBorderColor() );
}

} //bv
