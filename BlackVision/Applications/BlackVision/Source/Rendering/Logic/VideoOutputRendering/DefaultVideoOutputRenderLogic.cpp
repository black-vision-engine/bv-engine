#include "DefaultVideoOutputRenderLogic.h"


namespace bv {

// *********************************
//
DefaultVideoOutputRenderLogic::DefaultVideoOutputRenderLogic   ( bool useVideoCard, bool displayVideoOutputOnPreview )
    : m_useVideoCard( useVideoCard )
    , m_displayVideoOutputOnPreview( displayVideoOutputOnPreview )
{

}

// *********************************
//
DefaultVideoOutputRenderLogic::~DefaultVideoOutputRenderLogic  ()
{
}

// *********************************
//
void    DefaultVideoOutputRenderLogic::FrameRendered            ( Renderer * renderer, OffscreenRenderLogic * offscreenRenderLogic )
{
    { renderer; }
    { offscreenRenderLogic; }
}

} //bv
