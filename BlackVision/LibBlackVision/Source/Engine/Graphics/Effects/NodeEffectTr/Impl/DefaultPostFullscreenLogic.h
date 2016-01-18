#pragma once

#include "Rendering/Logic/NodeEffect/PostFullScreenEffectLogic.h"


namespace bv {

class DefaultPostFullscreenLogic : public PostFullScreenEffectLogic
{
private:

    unsigned int    m_firstChild;

public:

                    DefaultPostFullscreenLogic ();

            void    SetFirstChild                   ( unsigned int i );

    virtual void    Render                          ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) override;

};

} //bv
