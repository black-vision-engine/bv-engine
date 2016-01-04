#pragma once

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Shaders/RenderableEffect.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamInt.h"
#include "Engine/Graphics/Shaders/Parameters/ShaderParamVec4.h"


namespace bv {

class MixChannelsEffect : public RenderableEffect
{
private:

    ValueIntPtr     m_channelMixMaskVal;
    ValueVec4Ptr    m_channelMaskMaskVal;

    ShaderParamInt *  m_mixMaskParam;
    ShaderParamVec4 * m_maskMaskParam;

    unsigned char   m_rIdx;
    unsigned char   m_gIdx;
    unsigned char   m_bIdx;
    unsigned char   m_aIdx;

    glm::vec4       m_maskMask;

public:

                        MixChannelsEffect   ( Texture2DPtr tex );
                        ~MixChannelsEffect  ();

    void                SetRIdx         ( unsigned char idx );
    void                SetGIdx         ( unsigned char idx );
    void                SetBIdx         ( unsigned char idx );
    void                SetAIdx         ( unsigned char idx );

    void                SetMask         ( const glm::vec4 & mask );

    unsigned char       GetRIdx         () const;
    unsigned char       GetGIdx         () const;
    unsigned char       GetBIdx         () const;
    unsigned char       GetAIdx         () const;

    glm::vec4           GetMask         () const;

private:

    PixelShader *       CreatePS        ( Texture2DPtr tex );
    VertexShader *      CreateVS        ();

    int                 GetChannelMask  () const;

};

DEFINE_PTR_TYPE(MixChannelsEffect)
DEFINE_CONST_PTR_TYPE(MixChannelsEffect)

} //bv
