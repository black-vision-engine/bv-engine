#pragma once

#include "Engine/Graphics/Effects/NodeEffect/PreFullScreenEffectLogic.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class InterlacePreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
private:

    unsigned int    m_firstBufferIdx;

public:

                                        InterlacePreFullscreenEffectLogic   ( unsigned int firstBufStartIdx );

    virtual void                        Render                              ( SceneNode * node, RenderLogicContext * ctx, const std::vector< RenderTarget * > & outputs ) override;

    virtual std::vector< IValuePtr >    GetValues                           () const override;

};

} //bv
