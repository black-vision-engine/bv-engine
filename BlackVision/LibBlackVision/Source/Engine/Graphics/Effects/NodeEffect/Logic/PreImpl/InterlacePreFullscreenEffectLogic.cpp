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
void                        InterlacePreFullscreenEffectLogic::RenderImpl   ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs )
{
    { outputs; }
    { node; }
    { ctx; }

    // FIXME: implement using copies of frames
}

// *********************************
//
std::vector< IValuePtr >    InterlacePreFullscreenEffectLogic::GetValues    () const
{
    return std::vector< IValuePtr >();
}

} //bv
