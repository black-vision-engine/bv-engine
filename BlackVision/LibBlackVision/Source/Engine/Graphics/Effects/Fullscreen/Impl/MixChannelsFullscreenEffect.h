#pragma once

#include "Rendering/Logic/FullScreen/FullscreenEffect.h"

#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class MixChannelsFullscreenEffect : public FullscreenEffect
{
private:

    ValueIntPtr     m_channelMaskVal;

    unsigned char   m_rIdx;
    unsigned char   m_gIdx;
    unsigned char   m_bIdx;
    unsigned char   m_aIdx;

public:

                                MixChannelsFullscreenEffect     ( unsigned char rIdx = 0, unsigned char gIdx = 1, unsigned char bIdx = 2, unsigned char aIdx = 3 );
                                ~MixChannelsFullscreenEffect    ();

    void                        SetRIdx                         ( unsigned char idx );
    void                        SetGIdx                         ( unsigned char idx );
    void                        SetBIdx                         ( unsigned char idx );
    void                        SetAIdx                         ( unsigned char idx );

    unsigned char               GetRIdx                         () const;
    unsigned char               GetGIdx                         () const;
    unsigned char               GetBIdx                         () const;
    unsigned char               GetAIdx                         () const;

protected:

    virtual RenderableEntity *  CreateFullscreenQuad            () const override;

private:

    PixelShader *               CreatePS                        () const;

    int                         GetChannelMask                  () const;

};

DEFINE_PTR_TYPE(MixChannelsFullscreenEffect)
DEFINE_CONST_PTR_TYPE(MixChannelsFullscreenEffect)

} //bv
