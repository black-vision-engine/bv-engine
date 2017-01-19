#include "stdafx.h"

#include "OutputStreamSharedMem.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
OutputStreamSharedMem::OutputStreamSharedMem   ( unsigned int width, unsigned int height, unsigned int shmScaleFactor )
    : OutputInstance( width, height )
{
    m_shmVideoBuffer = new SharedMemoryVideoBuffer( width, height, TextureFormat::F_A8R8G8B8, shmScaleFactor );
}

// *********************************
//
OutputStreamSharedMem::~OutputStreamSharedMem  ()
{
    delete m_shmVideoBuffer;
}

// *********************************
//
void    OutputStreamSharedMem::ProcessFrameData  ( NRenderContext * ctx, RenderResult * result )
{
    // FIXME: nrl - use result to cache resacaled textures between outputs - it is a valid place as it is still a rendering result (e.g. when SD output is exactly the same as SHM output)
    // FIXME: nrl - neither effecto nor readback are necessary in such case

    auto rct = GetActiveRenderChannel();
    assert( result->IsActive( rct ) && result->ContainsValidData( rct ) );

    // auto outputRT = result->GetActiveRenderTarget( rct );
    
    assert( false ); // FIXME: nrl - implement

    auto tex = result->ReadColorTexture( renderer( ctx ), rct );

    m_shmVideoBuffer->PushFrame( tex );
}

} //nrl
} //bv
