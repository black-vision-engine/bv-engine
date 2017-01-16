#include "stdafx.h"

#include "OutputStreamSharedMem.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
OutputStreamSharedMem::OutputStreamSharedMem   ( unsigned int width, unsigned int height, unsigned int shmScaleFactor )
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
    // FIXME: nrl - default logic uses only RenderChannelType::RCT_OUTPUT_1 result channel to show the results
    // FIXME: ntl - implement more logic here
    auto rct = RenderChannelType::RCT_OUTPUT_1;
    assert( result->IsActive( rct ) && result->ContainsValidData( rct ) );
    
    auto tex = result->ReadColorTexture( renderer( ctx ), rct );

    m_shmVideoBuffer->PushFrame( tex );
}

} //nrl
} //bv
