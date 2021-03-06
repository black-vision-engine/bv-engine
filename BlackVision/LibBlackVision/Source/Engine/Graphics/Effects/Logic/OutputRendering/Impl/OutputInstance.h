#pragma once

#include "Engine/Graphics/Effects/Logic/State/OutputState.h"

#include "Engine/Graphics/Effects/Logic/OutputRendering/Output.h"


namespace bv { 

class FrameDataHandler;

// FIXME: nrl - Right now it uses basic state in this class, when the whole output subsystem is designed, a separated mechanism should be used to configure it
// FIXME: nrl - as well as additional static config mechanism (there are compile time params which can be changed only in the code, e.g. numTrackedFrameBuffers and static params which are read from the xml
// FIXME: nrl - and dynamic params which can be changed in runtime
class OutputInstance : public Output
{
protected:

    OutputState        m_state;

    FrameDataHandler *  m_frameDataHandler;

    bool                m_isEnabled;

public:

                        OutputInstance          ( unsigned int width, unsigned int height, FrameDataHandler * handler );
                        ~OutputInstance         ();

    virtual void        ProcessFrameData        ( RenderContext * ctx, RenderedChannelsData * input ) override;

    bool                IsEnabled               () const;
    void                Enable                  ();
    void                Disable                 ();

    OutputState &      AccessOutputState       ();

private:

    void                SyncHandlerState        ();

};


} //bv
