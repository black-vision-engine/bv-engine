#include "DefaultEffect.h"

#include <cassert>

#include "Engine/Interfaces/IShaderDataSource.h"

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
    delete GetPass( 0 )->GetPixelShader();
    delete GetPass( 0 )->GetVertexShader();
    delete GetPass( 0 )->GetGeometryShader();
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
//
void               DefaultEffect::AddTextures       ( Shader * shader, const ITexturesData * txData )
{
    if ( shader )
    {
        auto textures   = txData->GetTextures   ();
        auto animations = txData->GetAnimations ();

        for( auto tx : textures )
        {
                SamplerWrappingMode wp[] = {
                                                ConstantsMapper::EngineConstant( tex->m_wrappingModeX ) 
                                            ,   ConstantsMapper::EngineConstant( tex->m_wrappingModeY )
                                            ,   SamplerWrappingMode::SWM_REPEAT // FIXME: Add 3d texture support
                                            }; 
                //FIXME: jak to kurwa przez tex->m_texName ????
                auto textureSampler = new TextureSampler(       i
                                                            ,   tex->m_texName
                                                            ,   bv::SamplerSamplingMode::SSM_MODE_2D
                                                            ,   ConstantsMapper::EngineConstant( tex->m_filteringMode )
                                                            ,   wp
                                                            ,   tex->m_texBorderColor.Evaluate( 0.f ) );
                effect->GetPass( 0 )->GetPixelShader()->AddTextureSampler( textureSampler );

                auto loadedTex = bv::GTextureManager.LoadTexture( tex->m_resHandle, false );
                auto shaderParams = effect->GetPass( 0 )->GetPixelShader()->Parameters();
                shaderParams->AddTexture( loadedTex );

                i++;
        }
    }
}

// *********************************
//
ShaderParameters * DefaultEffect::DefaultParamsImpl ( const IShaderDataSource * ds ) const
{
    ShaderParameters * sp = nullptr;

    if( ds != nullptr )
    {
        //Register parameters
        sp = new ShaderParameters();

        for( auto value : ds->GetValues() )
        {
            GenericShaderParam * param = ShaderParamFactory::CreateGenericParameter( value );
            assert( param != nullptr );

            sp->AddParameter( param );
        }
    }
    /*
        //Register textures and animations
        auto textures   = ds->GetTexturesData()->GetTextures();
        auto animations = ds->GetTexturesData()->GetAnimations();

        for( auto tx : textures )
        {
        }

        if( animations.size() > 0 )
        {
            for( auto anim : animations )
            {
            }
        }

        for( auto txData : ds->
                    int i = 0;
            for( auto tex : finalizer->GetTextures() )
            {
                SamplerWrappingMode wp[] = {
                                                ConstantsMapper::EngineConstant( tex->m_wrappingModeX ) 
                                            ,   ConstantsMapper::EngineConstant( tex->m_wrappingModeY )
                                            ,   SamplerWrappingMode::SWM_REPEAT // FIXME: Add 3d texture support
                                            }; 
                //FIXME: jak to kurwa przez tex->m_texName ????
                auto textureSampler = new TextureSampler(       i
                                                            ,   tex->m_texName
                                                            ,   bv::SamplerSamplingMode::SSM_MODE_2D
                                                            ,   ConstantsMapper::EngineConstant( tex->m_filteringMode )
                                                            ,   wp
                                                            ,   tex->m_texBorderColor.Evaluate( 0.f ) );
                effect->GetPass( 0 )->GetPixelShader()->AddTextureSampler( textureSampler );

                auto loadedTex = bv::GTextureManager.LoadTexture( tex->m_resHandle, false );
                auto shaderParams = effect->GetPass( 0 )->GetPixelShader()->Parameters();
                shaderParams->AddTexture( loadedTex );

                i++;
            }



    }
    */
    return sp;

}

} //bv
