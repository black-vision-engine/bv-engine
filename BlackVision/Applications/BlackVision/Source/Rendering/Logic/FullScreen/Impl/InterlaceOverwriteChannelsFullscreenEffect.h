#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class InterlaceOverwriteChannelsFullscreenEffect : public FullscreenEffect
{
private:


    ValueIntPtr     m_startEvenValue;
    ValueIntPtr     m_heightValue;

    ValueIntPtr     m_channelMaskVal;
    ValueFloatPtr   m_alphaVal;
    ValueIntPtr     m_overwriteAlphaVal;

    bool            m_startEven;
    int             m_height;

    unsigned char   m_rIdx;
    unsigned char   m_gIdx;
    unsigned char   m_bIdx;
    unsigned char   m_aIdx;

    float           m_alpha;

    bool            m_overwriteAlpha;

public:

                                InterlaceOverwriteChannelsFullscreenEffect  ( unsigned char rIdx = 0, unsigned char gIdx = 1, unsigned char bIdx = 2, unsigned char aIdx = 3, float alpha = 1.0f, bool startEven = false, int height = 1080 );
                                ~InterlaceOverwriteChannelsFullscreenEffect ();

    void                        SetInterlaceTextures                        ( Texture2DPtr tex0, Texture2DPtr tex1 );

    void                        SetStartEven                                ( bool startEven );
    bool                        GetStartEven                                () const;

    void                        SetHeight                                   ( int height );
    int                         GetHeight                                   () const;

    void                        SetRIdx                                     ( unsigned char idx );
    void                        SetGIdx                                     ( unsigned char idx );
    void                        SetBIdx                                     ( unsigned char idx );
    void                        SetAIdx                                     ( unsigned char idx );

    unsigned char               GetRIdx                                     () const;
    unsigned char               GetGIdx                                     () const;
    unsigned char               GetBIdx                                     () const;
    unsigned char               GetAIdx                                     () const;

    void                        SetAlpha                                    ( float alpha );
    float                       GetAlpha                                    () const;

    void                        SetOverwriteAlpha                           ( bool overwriteAlpha );
    bool                        GetOverwriteAlpha                           () const;

protected:

    virtual RenderableEntity *  CreateFullscreenQuad                () const override;

private:

    PixelShader *               CreatePS                            () const;

    int                         GetChannelMask                      () const;

};

DEFINE_PTR_TYPE(InterlaceOverwriteChannelsFullscreenEffect)
DEFINE_CONST_PTR_TYPE(InterlaceOverwriteChannelsFullscreenEffect)

} //bv
