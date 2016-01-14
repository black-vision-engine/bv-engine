#pragma once

#include "Rendering/Logic/NodeEffect/PreFullScreenEffectLogic.h"


namespace bv {

class AlphaMaskPreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
private:

    RenderTarget *  m_intRT;

public:

                    AlphaMaskPreFullscreenEffectLogic   ();

    virtual void    Render                              ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) override;

};

} //bv
