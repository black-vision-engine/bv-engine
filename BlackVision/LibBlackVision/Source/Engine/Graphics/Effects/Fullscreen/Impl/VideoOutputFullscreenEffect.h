#pragma once

#include "Engine/Graphics/Effects/FullScreen/FullscreenEffect.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class VideoOutputFullscreenEffect : public FullscreenEffect
{
private:

    mutable PixelShader *   m_pixelShader;

    ValueIntPtr     m_useInterlaceValue;
    ValueIntPtr     m_startEvenValue;
    ValueFloatPtr   m_heightValue;

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

    bool            m_useInterlace;
    bool            m_overwriteAlpha;

public:

                                VideoOutputFullscreenEffect                 ( Texture2DPtr tex0, Texture2DPtr tex1 );
                                ~VideoOutputFullscreenEffect                ();

    void                        SwapTextures                                ();

    void                        SetInterlaceEnabled                         ( bool enabled );
    bool                        GetInterlaceEnabled                         () const;

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

    PixelShader *               CreatePS                            ( Texture2DPtr tex0, Texture2DPtr tex1 ) const;

    int                         GetChannelMask                      () const;

};

DEFINE_PTR_TYPE(VideoOutputFullscreenEffect)
DEFINE_CONST_PTR_TYPE(VideoOutputFullscreenEffect)

} //bv
