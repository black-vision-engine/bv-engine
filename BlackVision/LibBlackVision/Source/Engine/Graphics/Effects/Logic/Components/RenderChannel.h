#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Textures/Texture2D.h"


namespace bv { 

class RenderTargetStackAllocator;
class RenderTarget;
class Renderer;



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

    mutable Texture2DPtr            m_cachedReadbackTexture;
    mutable bool                    m_cachedReadbackUpToDate;

    mutable std::set< const audio::AudioEntity * > m_renderChannelAudioEntities;

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

    Texture2DPtr            ReadColorTexture            ( Renderer * renderer ) const;
    void                    InvalidateCachedTexture     () const;

    std::set< const audio::AudioEntity * > & AccessRenderChannelAudioEntities() const;
};

} //bv
