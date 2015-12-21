#include "VideoOutputRenderLogic.h"

#include "Rendering/Logic/FullScreen/Impl/InterlaceOverwriteChannelsFullscreenEffect.h"


namespace bv {

// *********************************
//
VideoOutputRenderLogic::VideoOutputRenderLogic          ( unsigned int height, bool interlaceOdd )
    : m_height( height )
    , m_effect( nullptr )
    , m_interlaceOdd( interlaceOdd )
{
}

// *********************************
//
VideoOutputRenderLogic::~VideoOutputRenderLogic         ()
{
}

// *********************************
//
void    VideoOutputRenderLogic::Render                  ( )
{
    //Get last two frames
    //Apply super kewl ultra effect
    //Render to specified target
}

// *********************************
//
InterlaceOverwriteChannelsFullscreenEffect *  VideoOutputRenderLogic::AccessEffect  ()
{
    if ( !m_effect )
    {
        m_effect = new InterlaceOverwriteChannelsFullscreenEffect();
    }

    return m_effect;        
}

} //bv
