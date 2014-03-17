#include "DefaultEffect.h"

#include <cassert>

#include "Engine/Interfaces/IShaderDataSource.h"

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Models/Interfaces/IAnimationDescriptor.h"

#include "Engine/Models/Plugins/ConstantsMapper.h"

#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"

#include "Engine/Graphics/Shaders/Parameters/ShaderParameters.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamFactory.h"


namespace bv {

// *********************************
//
DefaultEffect::DefaultEffect    ( const IShaderDataSource * psds, const IShaderDataSource * vsds, const IShaderDataSource * gsds )
{
    assert( psds != nullptr );
    assert( vsds != nullptr );

    ShaderParameters * psparams = DefaultParamsPS( psds );
    ShaderParameters * vsparams = DefaultParamsVS( vsds );

    assert( psparams != nullptr );
    assert( vsparams != nullptr );

    PixelShader * ps    = new PixelShader( psds->GetShaderSource(), psparams );
    VertexShader * vs   = new VertexShader( vsds->GetShaderSource(), vsparams );

    AddTextures( ps, psds->GetTexturesData() );
    AddTextures( vs, vsds->GetTexturesData() );

    GeometryShader * gs = nullptr;
    if ( gsds != nullptr )
    {
        ShaderParameters * gsparams = DefaultParamsGS( gsds );

        assert( gsparams != nullptr );

        gs = new GeometryShader( gsds->GetShaderSource(), gsparams );

        AddTextures( vs, gsds->GetTexturesData() );
    }

    RenderablePass * pass = new RenderablePass( ps, vs, gs );
    AddPass( pass );
}

// *********************************
//
DefaultEffect::~DefaultEffect   ()
{
    assert( NumPasses() == 1 );

    auto pass = GetPass( 0 );

    delete pass->GetPixelShader();
    delete pass->GetVertexShader();
    delete pass->GetGeometryShader();
}

// *********************************
//
ShaderParameters * DefaultEffect::DefaultParamsPS  ( const IShaderDataSource * ds ) const
{
    return DefaultParamsImpl( ds );
}

// *********************************
//
ShaderParameters * DefaultEffect::DefaultParamsVS  ( const IShaderDataSource * ds ) const
{
    auto params = DefaultParamsImpl( ds );

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
ShaderParameters * DefaultEffect::DefaultParamsGS  ( const IShaderDataSource * ds ) const
{
    if( ds )
    {
        return DefaultParamsVS( ds );
    }

    return nullptr;
}

// *********************************
// FIXME: only 2D textures right now
void               DefaultEffect::AddTextures       ( Shader * shader, const ITexturesData * txData )
{
    unsigned int samplerNum = 0;

    if ( shader )
    {
        auto params     = shader->GetParameters ();
        auto textures   = txData->GetTextures   ();
        auto animations = txData->GetAnimations ();

        for( auto tx : textures )
        {
            auto sampler = CreateSampler( tx, samplerNum );
            auto texture = CreateTexture( tx );

            shader->AddTextureSampler( sampler );
            params->AddTexture( texture );

            samplerNum++;
        }

        for( auto anim : animations )
        {
            if( anim->NumTextures() > 0 )
            {
                auto sampler    = CreateSampler( anim, samplerNum );
                auto sequence   = CreateSequence( anim );

                shader->AddTextureSampler( sampler );
                params->AddTexture( sequence );

                samplerNum++;
            }
        }
    }
}

// *********************************
//
ShaderParameters *      DefaultEffect::DefaultParamsImpl ( const IShaderDataSource * ds ) const
{
    ShaderParameters * sp = nullptr;

    if( ds != nullptr )
    {
        sp = new ShaderParameters();

        for( auto value : ds->GetValues() )
        {
            GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( value );
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
    auto wrapX          = ConstantsMapper::EngineConstant( txParams->GetWrappingModeX() );
    auto wrapY          = ConstantsMapper::EngineConstant( txParams->GetWrappingModeY() );            
    auto samplingMode   = SamplerSamplingMode::SSM_MODE_2D; //FIXME: only 2D textures right now
    auto filteringMode  = ConstantsMapper::EngineConstant( txParams->GetFilteringMode() );
    auto borderColor    = txParams->DefaultBorderColor();

    SamplerWrappingMode wrappingMode[] = { wrapX, wrapY, SamplerWrappingMode::SWM_REPEAT };

    auto sampler = new TextureSampler( samplerNum, txParams->GetName(), samplingMode, filteringMode, wrappingMode, borderColor ); 

    return sampler;
}

// *********************************
//
Texture2DImpl *         DefaultEffect::CreateTexture       ( const ITextureDescriptor * txParams ) const
{
    auto format = txParams->GetFormat();
    auto width  = txParams->GetWidth();
    auto height = txParams->GetHeight();

    auto texture = new Texture2DImpl( format, width, height );
    texture->WriteBits( txParams->GetBits(), format, width, height );

    return texture;
}

// *********************************
//
Texture2DSequenceImpl * DefaultEffect::CreateSequence       ( const IAnimationDescriptor * animParams ) const
{
    auto format = animParams->GetFormat();
    auto width  = animParams->GetWidth();
    auto height = animParams->GetHeight();

    auto sequence   = new Texture2DSequenceImpl( format, width, height );
            
    for( unsigned int i = 0; i < animParams->NumTextures(); ++i )
    {
        bool bAdded = sequence->AddTextureWritingBits( animParams->GetBits( i ), format, width, height );
        assert( bAdded );
    }

    return sequence;
}

} //bv
