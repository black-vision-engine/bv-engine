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
State * FromCtx( const Ctx * ctx )
{
    State * s = new State();
    RendererStatesBuilder::Assign( s, ctx );

    return s;
}

} //anonymous

// *********************************
//
AlphaState *    RendererStatesBuilder::CreateStateFromCtx( const model::AlphaContext * ac )
{
    return FromCtx< AlphaState >( ac );
}

// *********************************
//
CullState *     RendererStatesBuilder::CreateStateFromCtx( const model::CullContext * cc ) 
{
    return FromCtx< CullState >( cc );
}

// *********************************
//
DepthState *    RendererStatesBuilder::CreateStateFromCtx( const model::DepthContext * dc )
{
    return FromCtx< DepthState >( dc );
}

// *********************************
//
FillState *     RendererStatesBuilder::CreateStateFromCtx( const model::FillContext * fc )
{
    return FromCtx< FillState >( fc );
}

// *********************************
//
AlphaState *    RendererStatesBuilder::CreateDefaultAlphaState  ()
{
    return new AlphaState();
}

// *********************************
//
CullState *     RendererStatesBuilder::CreateDefaultCullState   ()
{
    return new CullState();
}

// *********************************
//
DepthState *    RendererStatesBuilder::CreateDefaultDepthState  ()
{
    return new DepthState();
}

// *********************************
//
FillState *     RendererStatesBuilder::CreateDefaultFillState   ()
{
    return new FillState();
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
void RendererStatesBuilder::Assign  ( AlphaState * as, const model::AlphaContext * ac )
{
    as->blendEnabled = ac->blendEnabled;
    as->blendColor = ac->blendColor;
    as->srcBlendMode = GetAlphaSrcBlendMode( ac->srcBlendMode );
    as->dstBlendMode = GetAlphaDstBlendMode( ac->dstBlendMode );
}

// *********************************
//
void RendererStatesBuilder::Assign  ( CullState * cs, const model::CullContext * cc )
{
    cs->enabled = cc->enabled;
    cs->isCCWOrdered = cc->isCCWOrdered;
}

// *********************************
//
void RendererStatesBuilder::Assign  ( DepthState * ds, const model::DepthContext * dc )
{
    ds->enabled = dc->enabled;
    ds->writable = dc->writable;
}

// *********************************
//
void RendererStatesBuilder::Assign  ( FillState * fs, const model::FillContext * fc )
{
    fs->fillMode = GetFillMode( fc->fillMode );
}

}
