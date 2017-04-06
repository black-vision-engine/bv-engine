#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/State/OutputState.h"


namespace bv { namespace nrl {

enum class CustomOutputType : unsigned int
{
    COT_PREVIEW = 0,
    COT_VIDEO,
    COT_STREAM,

    COT_TOTAL
};

class RenderedChannelsData;
class RenderContext;
class OutputDesc;

// FIXME: nrl - Right now it uses basic state in this class, when the whole output subsystem is designed, a separated mechanism should be used to configure it
// FIXME: nrl - as well as additional static config mechanism (there are compile time params which can be changed only in the code, e.g. numTrackedFrameBuffers and static params which are read from the xml
// FIXME: nrl - and dynamic params which can be changed in runtime
class Output
{
public:

    virtual             ~Output                 ();

    virtual void        ProcessFrameData        ( RenderContext * ctx, RenderedChannelsData * input ) = 0;

public:

    static Output *     Create                  ( const OutputDesc & desc ); 

};

} //nrl
} //bv
