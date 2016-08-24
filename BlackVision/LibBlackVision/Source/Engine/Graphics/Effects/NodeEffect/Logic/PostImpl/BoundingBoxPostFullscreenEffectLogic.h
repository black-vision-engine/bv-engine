#pragma once

#include "Engine/Graphics/Effects/NodeEffect/Logic/PostFullScreenEffectLogic.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class BoundingBoxEffect;
class Renderer;


class BoundingBoxPostFullscreenEffectLogic : public PostFullscreenEffectLogic
{
private:
    ValueVec4Ptr                        m_colorValue;

    static std::unique_ptr< BoundingBoxEffect > m_effect;

public:

                                        BoundingBoxPostFullscreenEffectLogic  ();
                                        ~BoundingBoxPostFullscreenEffectLogic ();

    virtual void                        Render                              ( SceneNode * node, RenderLogicContext * ctx );

    virtual std::vector< IValuePtr >    GetValues                           () const override;

};

} //bv
