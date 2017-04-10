#include "stdafx.h"

#include "ZSortFinalizeStep.h"

#include "Engine/Graphics/Effects/Logic/Components/RenderContext.h"
#include "Engine/Graphics/Effects/Logic/NodeRendering/NodeRenderLogic.h"

#include "Engine/Graphics/Effects/Logic/State/RenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {
namespace nrl {

// **************************
//
ZSortFinalizeStep::ZSortFinalizeStep          ()
    : Parent( nullptr )
{
    auto state = std::make_shared< RenderComponentState >();

    state->AppendValue( ValuesFactory::CreateValueBool( "useSort" ) );

    Parent::SetState( state );
}

// **************************
//
void        ZSortFinalizeStep::Apply                   ( SceneNodeRepr * nodeRepr, RenderContext * ctx )
{
    assert( ctx->GetBoundRenderTarget() != nullptr );

    if( UseSort() )
    {
        NodeRenderLogic::RenderQueued( nodeRepr, ctx );
    }
    else
    {
        NodeRenderLogic::Render( nodeRepr, ctx );
    }
}

// **************************
//
bool        ZSortFinalizeStep::IsIdle                    ( SceneNodeRepr * ) const
{
    return false;
}

// ***********************
//
bool        ZSortFinalizeStep::UseSort()
{
    auto useSort = GetState()->GetValueAt( 0 );
    return QueryTypedValue< ValueBoolPtr >( useSort )->GetValue();
}

} // nrl
} // bv
