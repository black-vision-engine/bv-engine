#include "RendererStateUpdater.h"

#include <cassert>

#include "Engine/Models/Plugins/Channels/RendererContext/RendererContext.h"
#include "Engine/Graphics/State/StateInstance.h"

namespace bv {

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
void    RenderStateUpdater::Update         ( float t )
{
    //FIXME: implement
}

} //bv
