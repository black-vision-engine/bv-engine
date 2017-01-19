#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderChannel.h"


namespace bv { namespace nrl {

class NOutputState
{
private:

    unsigned int        m_width;
    unsigned int        m_height;

    unsigned int        m_channelMask;

    RenderChannelType   m_activeRenderChannel;

public:

                        NOutputState            ( unsigned int w, unsigned int h );

    void                SetWidth                ( unsigned int w );
    void                SetHeight               ( unsigned int h );
    
    void                SetChannelMask          ( unsigned int channelMask );
    void                SetChannelMask          ( unsigned char rIdx, unsigned char gIdx, unsigned char bIdx, unsigned char aIdx );

    void                SetRIdx                 ( unsigned char rIdx );
    void                SetGIdx                 ( unsigned char gIdx );
    void                SetBIdx                 ( unsigned char bIdx );
    void                SetAIdx                 ( unsigned char aIdx );

    void                SetActiveRenderChannel  ( RenderChannelType activeRenderChannel );

    unsigned int        GetWidth                () const;
    unsigned int        GetHeight               () const;

    unsigned int        GetChannelMask          () const;
    unsigned int        GetRIdx                 () const;
    unsigned int        GetGIdx                 () const;
    unsigned int        GetBIdx                 () const;
    unsigned int        GetAIdx                 () const;

    RenderChannelType   GetActiveRenderChannel  () const;

private:

    void                SetMaskedUChar          ( unsigned int val, unsigned int numBits );
    unsigned int        GetMaskedUChar          ( unsigned int numBits ) const;

};

} //nrl
} //bv
