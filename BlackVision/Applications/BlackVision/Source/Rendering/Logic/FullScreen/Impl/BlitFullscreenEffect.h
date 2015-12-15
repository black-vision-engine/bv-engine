#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class BlitFullscreenEffect : public FullscreenEffect
{
private:

    bool            m_useAlpha;

    float           m_alpha;

    ValueFloatPtr   m_alphaVal;

public:

                                BlitFullscreenEffect    ( bool useAlpha );
                                ~BlitFullscreenEffect   ();

protected:

    virtual RenderableEntity *  CreateFullscreenQuad    () const override;

private:

    PixelShader *               CreatePS                () const;

};

} // bv
