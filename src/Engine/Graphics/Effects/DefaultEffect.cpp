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
    GeometryShader * gs = nullptr;

    if ( gsds != nullptr )
    {
        ShaderParameters * gsparams = DefaultParamsGS( gsds );

        assert( gsparams != nullptr );

        gs = new GeometryShader( gsds->GetShaderSource(), gsparams );
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
ShaderParameters * DefaultEffect::DefaultParamsImpl( const IShaderDataSource * ds ) const
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

} //bv
