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

                                BlitFullscreenEffect    ( Texture2DPtr tex, bool useAlpha );
                                ~BlitFullscreenEffect   ();

    float                       GetAlpha                () const;
    void                        SetAlpha                ( float alpha );

protected:

    virtual RenderableEntity *  CreateFullscreenQuad    () const override;

private:

    PixelShader *               CreatePS                ( Texture2DPtr tex ) const;

};

} // bv
