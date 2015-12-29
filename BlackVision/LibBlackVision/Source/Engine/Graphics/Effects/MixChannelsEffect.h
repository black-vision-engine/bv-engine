#pragma once

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamInt.h"


namespace bv {

class MixChannelsEffect : public RenderableEffect
{
private:

    ValueIntPtr     m_channelMaskVal;

    ShaderParamInt *  m_maskParam;

    unsigned char   m_rIdx;
    unsigned char   m_gIdx;
    unsigned char   m_bIdx;
    unsigned char   m_aIdx;

public:

                        MixChannelsEffect ();
                        ~MixChannelsEffect();

    void                SetRIdx         ( unsigned char idx );
    void                SetGIdx         ( unsigned char idx );
    void                SetBIdx         ( unsigned char idx );
    void                SetAIdx         ( unsigned char idx );

    unsigned char       GetRIdx         () const;
    unsigned char       GetGIdx         () const;
    unsigned char       GetBIdx         () const;
    unsigned char       GetAIdx         () const;

private:

    PixelShader *       CreatePS        ();
    VertexShader *      CreateVS        ();

    int                 GetChannelMask  () const;

};

DEFINE_PTR_TYPE(MixChannelsEffect)
DEFINE_CONST_PTR_TYPE(MixChannelsEffect)

} //bv
