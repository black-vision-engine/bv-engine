#pragma once

#include <vector>

#include "Engine/Types/Enums.h"

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Engine/Graphics/Effects/Utils/RenderTargetEntryDesc.h"


// Forward declarations and typedefs
namespace bv {

class Renderer;

typedef std::vector< RenderTarget * >  TRenderTargetVec;

} //bv 


// Main class declaration
namespace bv {

class RenderTargetStack
{
private:

    RenderTargetDataEntryDesc   m_renderTargetDescriptor;

    TRenderTargetVec            m_allocatedRenderTargets;

    bool                        m_isTopRTEnabled;

    int                         m_topActiveRenderTargetIdx;

public:

                        RenderTargetStack           ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~RenderTargetStack          ();

    void                AllocateNewRenderTarget     ( Renderer * renderer, RenderTarget::RTSemantic semantic );
    void                EnableTopRenderTarget       ( Renderer * renderer );
    void                DiscardCurrentRenderTarget  ( Renderer * renderer );
    void                DisableTopRenderTarget      ( Renderer * renderer );

    RenderTarget *      GetRenderTargetAt           ( int i ) const;

    unsigned int        TotalActiveRenderTargets    () const;
    unsigned int        TotalAllocatedRenderTargets () const;

}; 

} //bv
