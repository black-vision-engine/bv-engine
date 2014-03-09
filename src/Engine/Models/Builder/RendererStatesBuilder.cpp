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
void RendererStatesBuilder::Create( RendererStateInstance * inst, const model::RendererContext * ctx )
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

// *********************************
//
void RendererStatesBuilder::Assign  ( AlphaState * as, const model::AlphaContext * ac )
{
    as->blendEnabled = ac->blendEnabled;
    as->blendColor = ac->blendColor;
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
