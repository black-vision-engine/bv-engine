#pragma once

#include <vector>


namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;

namespace nrl {

enum class RenderChannelType : unsigned int
{
    RCT_OUTPUT_1 = 0,
    RCT_OUTPUT_2,
    RCT_OUTPUT_3,
    RCT_OUTPUT_4,

    RCT_TOTAL
};

//////////////////////////////////////
// Class used to store currently rendered render target along with the history of already rendered frames (right now only current and previous frames are stored)
class RenderChannel
{
private:

    std::vector< RenderTarget * >   m_renderTargets;

    int                             m_activeRenderTargetIdx;

    bool                            m_isActive;

public:

                            RenderChannel               ( RenderTargetStackAllocator * allocator, unsigned int numTrackedRenderTargets );
                            ~RenderChannel              ();

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
