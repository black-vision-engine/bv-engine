#pragma once

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/OutputInstance.h"

#include "Engine/Graphics/Rendering/SharedMemoryVideoBuffer.h"


namespace bv { namespace nrl {

class RenderResult;
class NRenderContext;

class OutputStreamSharedMem : public OutputInstance
{
private:

    SharedMemoryVideoBuffer *   m_shmVideoBuffer;

public:

                    OutputStreamSharedMem   ( unsigned int width, unsigned int height, unsigned int shmScaleFactor );
                    ~OutputStreamSharedMem  ();

    virtual void    ProcessFrameData        ( NRenderContext * ctx, RenderResult * result ) override;

};

} //nrl
} //bv
