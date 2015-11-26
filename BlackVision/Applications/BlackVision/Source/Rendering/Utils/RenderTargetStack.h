#pragma once

#include <vector>

#include "Engine/Types/Enums.h"

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Resources/RenderTarget.h"

#include "Rendering/Utils/RenderTargetEntryDesc.h"


// Forward declarations and typedefs
namespace bv {

class Renderer;

typedef std::vector< RenderTarget * >  RenderTargetVec;

} //bv 


// Main class declaration
namespace bv {

class RenderTargetStack
{
private:

    RenderTargetDataEntryDesc   m_renderTargetDescriptor;

    RenderTargetVec             m_allocatedRenderTargets;

    bool                        m_isTopRTEnabled;

    int                         m_topActiveRenderTargetIdx;

    unsigned int                m_curDisplayTarget;
    unsigned int                m_buffersPerTarget;

public:

                        RenderTargetStack           ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~RenderTargetStack          ();

    void                AllocateNewRenderTarget     ( Renderer * renderer, RenderTarget::RTSemantic semantic );
    void                EnableTopRenderTarget       ( Renderer * renderer );
    void                DiscardCurrentRenderTarget  ( Renderer * renderer );
    void                DisableTopRenderTarget      ( Renderer * renderer );

    RenderTarget *      GetRenderTargetAt           ( int i ) const;

}; 

} //bv
