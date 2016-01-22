#include "DefaultPostFullscreenLogic.h"


namespace bv {

    // *********************************
//
void                        DefaultPostFullscreenLogic::Render      ( SceneNode * node, RenderLogicContext * ctx )
{
    { node; }
    { ctx; }

    //Empty implementation - by design
}

// *********************************
//
std::vector< IValuePtr >    DefaultPostFullscreenLogic::GetValues   () const
{
    return std::vector< IValuePtr >();
}

} //bv
