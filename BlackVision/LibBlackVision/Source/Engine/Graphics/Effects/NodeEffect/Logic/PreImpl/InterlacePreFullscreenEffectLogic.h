#pragma once

#include "Engine/Graphics/Effects/NodeEffect/Logic/PreFullScreenEffectLogic.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class InterlacePreFullscreenEffectLogic : public PreFullscreenEffectLogic
{
private:

    unsigned int    m_firstBufferIdx;

public:

                                        InterlacePreFullscreenEffectLogic   ( unsigned int firstBufStartIdx );

protected:

    virtual void                        RenderImpl                          ( SceneNode * node, RenderLogicContext * ctx, std::vector< RenderTarget * > & outputs ) override;

public:

    virtual std::vector< IValuePtr >    GetValues                           () const override;

};

} //bv
