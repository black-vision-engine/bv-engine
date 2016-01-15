#pragma once

#include <vector>

#include "Engine/Types/Enums.h"

#include "Engine/Graphics/Resources/Texture2D.h"
#include "Engine/Graphics/Resources/RenderTarget.h"


namespace bv {

class RenderTargetStackAllocator
{
private:

    std::vector< RenderTarget * >   m_allocatedRenderTargets;

    unsigned int                    m_width;
    unsigned int                    m_height;

    TextureFormat                   m_fmt;

    int                             m_topIdx;

public:

                        RenderTargetStackAllocator  ( unsigned int width, unsigned int height, TextureFormat fmt = TextureFormat::F_A8R8G8B8 );
                        ~RenderTargetStackAllocator ();

    RenderTarget *      Allocate                    ( RenderTarget::RTSemantic semantic );
    RenderTarget *      Top                         ();

    bool                Free                        ();

    unsigned int        GetTopIndex                 () const;

    RenderTarget *      CreateRenderTarget          ( RenderTarget::RTSemantic semantic );

}; 

} //bv
