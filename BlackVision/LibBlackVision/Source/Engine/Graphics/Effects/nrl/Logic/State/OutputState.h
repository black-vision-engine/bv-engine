#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"


namespace bv { namespace nrl {

class OutputState
{
private:

    unsigned int        m_width;
    unsigned int        m_height;

    unsigned int        m_channelMapping;
    
    bool                m_rMask;
    bool                m_gMask;
    bool                m_bMask;
    bool                m_aMask;

    RenderChannelType   m_activeRenderChannel;

public:

                        OutputState                ( unsigned int w, unsigned int h );

    void                SetWidth                    ( unsigned int w );
    void                SetHeight                   ( unsigned int h );
    
    void                SetChannelMapping           ( unsigned int channelMapping );
    void                SetChannelMapping           ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx );

    void                SetRIdx                     ( unsigned char rIdx );
    void                SetGIdx                     ( unsigned char gIdx );
    void                SetBIdx                     ( unsigned char bIdx );
    void                SetAIdx                     ( unsigned char aIdx );

    void                SetActiveRenderChannelType  ( RenderChannelType activeRenderChannel );

    void                SetMaskState                ( bool rChannelEnabled, bool gChannelEnabled, bool bChannelEnabled, bool aChannelEnabled );

    void                SetRChannelState            ( bool isEnabled );
    void                SetGChannelState            ( bool isEnabled );
    void                SetBChannelState            ( bool isEnabled );
    void                SetAChannelState            ( bool isEnabled );

    unsigned int        GetWidth                    () const;
    unsigned int        GetHeight                   () const;

    unsigned int        GetChannelMapping           () const;

    unsigned int        GetRIdx                     () const;
    unsigned int        GetGIdx                     () const;
    unsigned int        GetBIdx                     () const;
    unsigned int        GetAIdx                     () const;

    RenderChannelType   GetActiveRenderChannelType  () const;

    glm::vec4           GetChannelMask              () const;

    bool                GetRChannelState            () const;
    bool                GetGChannelState            () const;
    bool                GetBChannelState            () const;
    bool                GetAChannelState            () const;

    bool                RepresentsDefaultTexture    () const;

private:

    void                SetMappedUChar              ( unsigned int val, unsigned int numBits );
    unsigned int        GetMappedUChar              ( unsigned int numBits ) const;

};

} //nrl
} //bv
