#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/NOutputState.h"


namespace bv { 
    
class RenderTarget;
    
namespace nrl {

enum class CustomOutputType : unsigned int
{
    COT_PREVIEW = 0,
    COT_VIDEO,
    COT_STREAM_SHM,
    COT_SCREENSHOT,

    COT_TOTAL
};

class RenderResult;
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

                        Output                  ( unsigned int width, unsigned int height ); // FIXME: nrl - pass resolution related parameters in a more generic way (config descriptor of some sort)
    virtual             ~Output                 ();

    virtual void        ProcessFrameData        ( NRenderContext * ctx, RenderResult * input ) = 0;

    bool                IsEnabled               () const;
    void                Enable                  ();
    void                Disable                 ();

    NOutputState &      AccessOutputState       ();

    // FIXME: nrl - convenience method, but unfortunately not a generic one
    RenderChannelType   GetActiveRenderChannel  () const;
    void                SetActiveRenderChannel  ( RenderChannelType rct );

    unsigned int        GetWidth                () const;
    unsigned int        GetHeight               () const;

    unsigned int        GetChannelMapping       () const;
    glm::vec4           GetChannelMask          () const;

};

} //nrl
} //bv
