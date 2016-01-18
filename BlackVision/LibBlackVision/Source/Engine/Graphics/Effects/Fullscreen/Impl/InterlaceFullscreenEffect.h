#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Resources/Texture2D.h"


namespace bv {

class InterlaceFullscreenEffect : public FullscreenEffect
{
private:

    bool            m_startEven;
    int             m_height;

    ValueIntPtr     m_startEvenValue;
    ValueIntPtr     m_heightValue;

public:

                                InterlaceFullscreenEffect   ( bool startEven, int height );
                                ~InterlaceFullscreenEffect  ();


    void                        SetInterlaceTextures        ( Texture2DPtr tex0, Texture2DPtr tex1 );

    void                        SetStartEven                ( bool startEven );
    bool                        GetStartEven                () const;

    void                        SetHeight                   ( int height );
    int                         GetHeight                   () const;

protected:

    virtual RenderableEntity *  CreateFullscreenQuad        () const override;

    PixelShader *               CreatePS                    () const;

};

DEFINE_PTR_TYPE(InterlaceFullscreenEffect)
DEFINE_CONST_PTR_TYPE(InterlaceFullscreenEffect)

} //bv
