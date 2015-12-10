#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class OverwriteChannelsFullscreenEffect : public FullscreenEffect
{
private:

    ValueIntPtr     m_channelMaskVal;
    ValueFloatPtr   m_alphaVal;
    ValueIntPtr     m_overwriteAlphaVal;

    unsigned char   m_rIdx;
    unsigned char   m_gIdx;
    unsigned char   m_bIdx;
    unsigned char   m_aIdx;

    float           m_alpha;

    bool            m_overwriteAlpha;

public:

                                OverwriteChannelsFullscreenEffect     ( unsigned char rIdx = 0, unsigned char gIdx = 1, unsigned char bIdx = 2, unsigned char aIdx = 3, float alpha = 1.0f );
                                ~OverwriteChannelsFullscreenEffect    ();

    void                        SetRIdx                             ( unsigned char idx );
    void                        SetGIdx                             ( unsigned char idx );
    void                        SetBIdx                             ( unsigned char idx );
    void                        SetAIdx                             ( unsigned char idx );

    unsigned char               GetRIdx                             () const;
    unsigned char               GetGIdx                             () const;
    unsigned char               GetBIdx                             () const;
    unsigned char               GetAIdx                             () const;

    void                        SetAlpha                            ( float alpha );

    float                       GetAlpha                            () const;

    void                        SetOverwriteAlpha                   ( bool overwriteAlpha );

    bool                        GetOverwriteAlpha                   () const;

protected:

    virtual RenderableEntity *  CreateFullscreenQuad                () const override;

private:

    PixelShader *               CreatePS                            () const;

    int                         GetChannelMask                      () const;

};

DEFINE_PTR_TYPE(OverwriteChannelsFullscreenEffect)
DEFINE_CONST_PTR_TYPE(OverwriteChannelsFullscreenEffect)

} //bv
