#pragma once

#include "Engine/Graphics/Effects/NodeEffect/Logic/PostFullScreenEffectLogic.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class NodeMaskPostFullscreenEffectLogic : public PostFullscreenEffectLogic
{
public:

                                        NodeMaskPostFullscreenEffectLogic   ();

    virtual void                        Render                              ( SceneNode * node, RenderLogicContext * ctx ) override;

    virtual std::vector< IValuePtr >    GetValues                           () const override;

};

} //bv
