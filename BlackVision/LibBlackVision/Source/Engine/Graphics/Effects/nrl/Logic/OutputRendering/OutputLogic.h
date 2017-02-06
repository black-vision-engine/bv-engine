#pragma once

#include <vector>

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderedChannelsData.h"
#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/Output.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/Initialization/OutputLogicDesc.h"


namespace bv { 
    
namespace nrl {

class NRenderContext;

class OutputLogic
{
private:

    std::vector< Output * >     m_outputs;

private:

                            OutputLogic             ();

public:

    virtual                 ~OutputLogic            ();

    // API directly related to frame rendering
    void                    ProcessFrameData        ( NRenderContext * ctx, RenderedChannelsData * rcd );

    // API relarted to global output state manipulation
    Output *                GetOutput               ( CustomOutputType outputType );

private:

    void                    AppendOutput            ( Output * output );

public:

    static OutputLogic *    Create                  ( OutputLogicDesc & desc );

};

} //nrl
} //bv
