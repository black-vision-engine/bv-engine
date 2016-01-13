#pragma once

#include "Rendering/Logic/NodeEffect/PreFullScreenEffectLogic.h"


namespace bv {

class SplitChildrenPreFullscreenLogic : public PreFullScreenEffectLogic
{
private:

    unsigned int    m_firstChild;

public:

                    SplitChildrenPreFullscreenLogic ();

            void    SetFirstChild                   ( unsigned int i );

    virtual void    Render                          ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) override;

};

} //bv
