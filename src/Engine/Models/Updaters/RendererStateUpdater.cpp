#include "RendererStateUpdater.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Graphics/State/StateInstance.h"
#include "Engine/Models/Builder/RendererStatesBuilder.h"

#include "Engine/Graphics/State/StateEnums.h"


namespace bv {

namespace {

// *******************************
//
void UpdateState( const model::AlphaContext * ac, AlphaState * as )
{
    if( ac )
    {
        assert( as );

        RendererStatesBuilder::Assign( as, ac );
    }
}

// *******************************
//
void UpdateState( const model::CullContext * cc, CullState * cs )
{
    if( cc )
    {
        assert( cs );

        RendererStatesBuilder::Assign( cs, cc );
    }

}

// *******************************
//
void UpdateState( const model::DepthContext * dc, DepthState * ds )
{
    if( dc )
    {
        assert( ds );

        RendererStatesBuilder::Assign( ds, dc );
    }

}

// *******************************
//
void UpdateState( const model::FillContext * fc, FillState * fs )
{
    if( fc )
    {
        assert( fs );

        RendererStatesBuilder::Assign( fs, fc );
    }
}

}

// *******************************
//
RenderStateUpdater::RenderStateUpdater     ( StateInstance * out , const model::RendererContext * in )
    : m_out( out )
    , m_in( in )
{
    assert( m_in );
    assert( m_out );
}

// *******************************
//
RenderStateUpdater::~RenderStateUpdater    ()
{
}

// *******************************
//
void    RenderStateUpdater::DoUpdate       ( TimeType t )
{
    if ( m_in->StateChanged() )
    {
        UpdateState( m_in->alphaCtx, RenderStateAccessor::AccessAlphaState( m_out ) );
        UpdateState( m_in->cullCtx, RenderStateAccessor::AccessCullState( m_out ) );
        UpdateState( m_in->depthCtx, RenderStateAccessor::AccessDepthState( m_out ) );
        UpdateState( m_in->fillCtx, RenderStateAccessor::AccessFillState( m_out ) );

        m_in->SetStateChanged( false );
    }
}

} //bv
