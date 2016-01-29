#include "stdafx.h"

#include "RendererStatesBuilder.h"

#include "Engine/Graphics/State/RendererStateInstance.h"
#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"


namespace bv {

namespace {

// *******************************
//
FillStateMode GetFillMode( model::FillContext::Mode mode )
{
    static FillStateMode modes[] = { FSM_POINTS, FSM_LINES, FSM_POLYGONS };

    return modes[ (int) mode ];
}

// *******************************
//
AlphaSrcBlendMode GetAlphaSrcBlendMode( model::AlphaContext::SrcBlendMode mode )
{
    static AlphaSrcBlendMode modes[] = { AlphaSrcBlendMode::ASBM_ZERO , AlphaSrcBlendMode::ASBM_ONE, AlphaSrcBlendMode::ASBM_DST_COLOR, AlphaSrcBlendMode::ASBM_ONE_MINUS_DST_COLOR, AlphaSrcBlendMode::ASBM_SRC_ALPHA, AlphaSrcBlendMode::ASBM_ONE_MINUS_SRC_ALPHA, AlphaSrcBlendMode::ASBM_DST_ALPHA,
                                         AlphaSrcBlendMode::ASBM_ONE_MINUS_DST_ALPHA, AlphaSrcBlendMode::ASBM_SRC_ALPHA_SATURATE, AlphaSrcBlendMode::ASBM_CONSTANT_COLOR, AlphaSrcBlendMode::ASBM_ONE_MINUS_CONSTANT_COLOR, AlphaSrcBlendMode::ASBM_CONSTANT_ALPHA, 
                                         AlphaSrcBlendMode::ASBM_ONE_MINUS_CONSTANT_ALPHA };

    return modes[ (int) mode ];
}

// *******************************
//
AlphaDstBlendMode GetAlphaDstBlendMode( model::AlphaContext::DstBlendMode mode )
{
    static AlphaDstBlendMode modes[] = { AlphaDstBlendMode::ADBM_ZERO, AlphaDstBlendMode::ADBM_ONE, AlphaDstBlendMode::ADBM_SRC_COLOR, AlphaDstBlendMode::ADBM_ONE_MINUS_SRC_COLOR,  AlphaDstBlendMode::ADBM_SRC_ALPHA,
                                         AlphaDstBlendMode::ADBM_ONE_MINUS_SRC_ALPHA, AlphaDstBlendMode::ADBM_DST_ALPHA, AlphaDstBlendMode::ADBM_ONE_MINUS_DST_ALPHA, AlphaDstBlendMode::ADBM_CONSTANT_COLOR,
                                         AlphaDstBlendMode::ADBM_ONE_MINUS_CONSTANT_COLOR, AlphaDstBlendMode::ADBM_CONSTANT_ALPHA, AlphaDstBlendMode::ADBM_ONE_MINUS_CONSTANT_ALPHA };

     return modes[ (int) mode ];
}

// *******************************
//
template< typename State, typename Ctx >
std::shared_ptr<State> FromCtx( const Ctx * ctx )
{
    std::shared_ptr<State> s = std::make_shared<State>();
    RendererStatesBuilder::Assign( s, ctx );

    return s;
}

} //anonymous

// *********************************
//
AlphaStatePtr    RendererStatesBuilder::CreateStateFromCtx( const model::AlphaContext * ac )
{
    return FromCtx< AlphaState >( ac );
}

// *********************************
//
CullStatePtr     RendererStatesBuilder::CreateStateFromCtx( const model::CullContext * cc ) 
{
    return FromCtx< CullState >( cc );
}

// *********************************
//
DepthStatePtr    RendererStatesBuilder::CreateStateFromCtx( const model::DepthContext * dc )
{
    return FromCtx< DepthState >( dc );
}

// *********************************
//
FillStatePtr     RendererStatesBuilder::CreateStateFromCtx( const model::FillContext * fc )
{
    return FromCtx< FillState >( fc );
}

// *********************************
//
AlphaStatePtr    RendererStatesBuilder::CreateDefaultAlphaState  ()
{
    return std::make_shared<AlphaState>();
}

// *********************************
//
CullStatePtr     RendererStatesBuilder::CreateDefaultCullState   ()
{
    return std::make_shared<CullState>();
}

// *********************************
//
DepthStatePtr    RendererStatesBuilder::CreateDefaultDepthState  ()
{
    return std::make_shared<DepthState>();
}

// *********************************
//
FillStatePtr     RendererStatesBuilder::CreateDefaultFillState   ()
{
    return std::make_shared<FillState>();
}

// *********************************
//
void RendererStatesBuilder::Create( RendererStateInstance * inst, model::RendererContextConstPtr ctx )
{
    if ( ctx->alphaCtx )
        inst->SetState( CreateStateFromCtx( ctx->alphaCtx ) );
    
    if ( ctx->cullCtx )
        inst->SetState( CreateStateFromCtx( ctx->cullCtx ) );

    if ( ctx->depthCtx )
        inst->SetState( CreateStateFromCtx( ctx->depthCtx ) );

    if ( ctx->fillCtx )
        inst->SetState( CreateStateFromCtx( ctx->fillCtx ) );

    //FIXME: skipping offset and stencil states for now
}

void RendererStatesBuilder::Create  ( RendererStateInstance * inst )
{
    inst->SetState( CreateDefaultAlphaState() );
    inst->SetState( CreateDefaultCullState() );
    inst->SetState( CreateDefaultDepthState() );
    inst->SetState( CreateDefaultFillState() );

    //FIXME: skipping offset and stencil states for now
}

// *********************************
//
void RendererStatesBuilder::Assign  ( AlphaStatePtr as, const model::AlphaContext * ac )
{
    as->blendEnabled = ac->blendEnabled;
    as->blendColor = ac->blendColor;
    as->srcRGBBlendMode = GetAlphaSrcBlendMode( ac->srcRGBBlendMode );
    as->dstRGBBlendMode = GetAlphaDstBlendMode( ac->dstRGBBlendMode );
	as->srcAlphaBlendMode = GetAlphaSrcBlendMode( ac->srcAlphaBlendMode );
	as->dstAlphaBlendMode = GetAlphaDstBlendMode( ac->dstAlphaBlendMode );
}

// *********************************
//
void RendererStatesBuilder::Assign  ( CullStatePtr cs, const model::CullContext * cc )
{
    cs->enabled = cc->enabled;
    cs->isCCWOrdered = cc->isCCWOrdered;
}

// *********************************
//
void RendererStatesBuilder::Assign  ( DepthStatePtr ds, const model::DepthContext * dc )
{
    ds->enabled = dc->enabled;
    ds->writable = dc->writable;
}

// *********************************
//
void RendererStatesBuilder::Assign  ( FillStatePtr fs, const model::FillContext * fc )
{
    fs->fillMode = GetFillMode( fc->fillMode );
}

}
