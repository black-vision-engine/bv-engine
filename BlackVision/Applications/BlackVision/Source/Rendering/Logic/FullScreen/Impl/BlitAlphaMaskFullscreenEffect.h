#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"


namespace bv {

class BlitAlphaMaskFullscreenEffect : public FullscreenEffect
{
private:

        ValueFloatPtr   m_alphaVal;

public:

        BlitAlphaMaskFullscreenEffect   ( Texture2DPtr tex, Texture2DPtr alpha );
        ~BlitAlphaMaskFullscreenEffect  ();

        void                SetAlpha    ( float alpha );

protected:

    virtual RenderableEntity *  CreateFullscreenQuad    () const override;

private:

    PixelShader *               CreatePS                ( Texture2DPtr tex, Texture2DPtr alpha ) const;

};

} // bv
