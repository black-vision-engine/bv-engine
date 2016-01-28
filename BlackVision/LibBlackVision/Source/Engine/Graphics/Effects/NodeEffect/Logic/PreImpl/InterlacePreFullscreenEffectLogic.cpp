#include "InterlacePreFullscreenEffectLogic.h"


namespace bv {

// *********************************
//
InterlacePreFullscreenEffectLogic::InterlacePreFullscreenEffectLogic        ( unsigned int firstBufStartIdx )
    : m_firstBufferIdx( firstBufStartIdx )
{
}

// *********************************
//
void                        InterlacePreFullscreenEffectLogic::Render       ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > * outputs )
{
    { node; }
    { ctx; }
    { outputs; }

    // FIXME: implement using copies of frames
}

// *********************************
//
std::vector< IValuePtr >    InterlacePreFullscreenEffectLogic::GetValues    () const
{
    return std::vector< IValuePtr >();
}

} //bv
