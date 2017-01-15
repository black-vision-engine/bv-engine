#pragma once

#include <vector>


namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;

namespace nrl {

enum class RenderOutputChannelType : unsigned int
{
    ROCT_OUTPUT_1 = 0,
    ROCT_OUTPUT_2,
    ROCT_OUTPUT_3,
    ROCT_OUTPUT_4,

    ROCT_TOTAL
};

//////////////////////////////////////
// Class used to store currently rendered render target along with the history of already rendered frames (right now only current and previous frames are stored)
class RenderOutputChannel
{
private:

    std::vector< RenderTarget * >   m_renderTargets;

    int                             m_activeRenderTargetIdx;

    bool                            m_isActive;

public:

                            RenderOutputChannel         ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargets );
                            ~RenderOutputChannel        ();

    unsigned int            GetNumRenderTargets         () const;

    const RenderTarget *    GetActiveRenderTarget       () const;
    const RenderTarget *    GetPreviousRenderTarget     () const;

    const RenderTarget *    GetRenderTarget             ( int reversedIdx ) const;

    void                    UpdateActiveRenderTargetIdx ();

    bool                    IsActive                    () const;
    void                    SetActiveFlag               ( bool isActive );

};

} //bv
} //nrl
