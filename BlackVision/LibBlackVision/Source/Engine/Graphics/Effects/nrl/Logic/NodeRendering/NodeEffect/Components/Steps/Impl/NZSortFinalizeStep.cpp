#include "stdafx.h"

#include "NZSortFinalizeStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {
namespace nrl {

// **************************
//
NZSortFinalizeStep::NZSortFinalizeStep          ()
    : Parent( nullptr )
{
    auto state = std::make_shared< NRenderComponentState >();

    state->AppendValue( ValuesFactory::CreateValueBool( "useSort" ) );

    Parent::SetState( state );
}

// **************************
//
void        NZSortFinalizeStep::Apply                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
{
    assert( ctx->GetBoundRenderTarget() != nullptr );

    if( UseSort() )
    {
        NNodeRenderLogic::RenderQueued( nodeRepr, ctx );
    }
    else
    {
        NNodeRenderLogic::Render( nodeRepr, ctx );
    }
}

// **************************
//
bool        NZSortFinalizeStep::IsIdle                    ( SceneNodeRepr * ) const
{
    return false;
}

// ***********************
//
bool        NZSortFinalizeStep::UseSort()
{
    auto useSort = GetState()->GetValueAt( 0 );
    return QueryTypedValue< ValueBoolPtr >( useSort )->GetValue();
}

} // nrl
} // bv
