#include "NodeEffectRenderLogic.h"


namespace bv {

// *********************************
//
NodeEffectRenderLogic::NodeEffectRenderLogic   ( RenderLogic * renderLogic, OffscreenRenderLogic * offscreenRenderLogic )
    : m_renderLogic( renderLogic )
    , m_offscreenRenderLogic( offscreenRenderLogic )
{
}

// *********************************
//
NodeEffectRenderLogic::~NodeEffectRenderLogic  ()
{
}

// *********************************
//
RenderLogic *           NodeEffectRenderLogic::GetRenderLogic          ()
{
    return m_renderLogic;
}

// *********************************
//
OffscreenRenderLogic *  NodeEffectRenderLogic::GetOffscreenRenderLogic ()
{
    return m_offscreenRenderLogic;
}

} //bv
