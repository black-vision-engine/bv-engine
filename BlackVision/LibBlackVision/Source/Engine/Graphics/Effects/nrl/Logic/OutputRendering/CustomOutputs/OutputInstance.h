#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NOutputState.h"


namespace bv { 
    
class RenderTarget;
    
namespace nrl {


class RenderedChannelsData;
class NRenderContext;
class NFullscreenEffect;

// FIXME: nrl - Right now it uses basic state in this class, when the whole output subsystem is designed, a separated mechanism should be used to configure it
// FIXME: nrl - as well as additional static config mechanism (there are compile time params which can be changed only in the code, e.g. numTrackedFrameBuffers and static params which are read from the xml
// FIXME: nrl - and dynamic params which can be changed in runtime
class Output
{
private:

    NOutputState    m_state;

    bool            m_isEnabled;

public:

                        Output                  ( unsigned int width, unsigned int height );
                        ~Output                 ();

    void                ProcessFrameData        ( NRenderContext * ctx, RenderedChannelsData * input );

    bool                IsEnabled               () const;
    void                Enable                  ();
    void                Disable                 ();

    NOutputState &      AccessOutputState       ();

    unsigned int        GetWidth                () const;
    unsigned int        GetHeight               () const;

};

} //nrl
} //bv
