#include "stdafx.h"

#include "NZSortPreFSEStep.h"

#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"
#include "Engine/Graphics/Effects/nrl/Logic/NodeRendering/NNodeRenderLogic.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NRenderComponentState.h"

#include "Engine/Graphics/SceneGraph/SceneNodeRepr.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Types/Values/ValuesFactory.h"


namespace bv {
namespace nrl {

// **************************
//
NZSortPreFSEStep::NZSortPreFSEStep          ()
    : Parent( nullptr )
{
    auto state = std::make_shared< NRenderComponentState >();

    state->AppendValue( ValuesFactory::CreateValueBool( "useSort" ) );

    Parent::SetState( state );
}

// **************************
//
unsigned int            NZSortPreFSEStep::GetNumOutputs               () const
{
    return 0;
}

// **************************
//
void                    NZSortPreFSEStep::ReadInputState              ()
{}

// **************************
//
void                    NZSortPreFSEStep::AllocateRenderTargets       ( NRenderContext * /*ctx*/ )
{
    // Effect doesn't use additional render targets
}

// **************************
//
const NRenderedData *   NZSortPreFSEStep::ApplyImpl                   ( SceneNodeRepr * nodeRepr, NRenderContext * ctx )
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

    return nullptr;
}

// **************************
//
bool    NZSortPreFSEStep::IsIdle                    ( SceneNodeRepr * ) const
{
    return false;
}

// **************************
// 
bool    NZSortPreFSEStep::IsFinal                   ( SceneNodeRepr * ) const
{
    return false;

}

// ***********************
//
bool    NZSortPreFSEStep::UseSort()
{
    auto useSort = GetState()->GetValueAt( 0 );
    return QueryTypedValue< ValueBoolPtr >( useSort )->GetValue();
}

} // nrl
} // bv
