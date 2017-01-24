#include "stdafx.h"

#include "OutputStreamSharedMem.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"

#include "LibImage.h"


namespace bv { namespace nrl {

// *********************************
//
OutputStreamSharedMem::OutputStreamSharedMem   ( unsigned int width, unsigned int height )
    : OutputInstance( width, height )
    , m_activeRenderOutput( 1 )
    , m_shmRT( nullptr )
    , m_shmTexture( nullptr )
{
    m_shmVideoBuffer = new SharedMemoryVideoBuffer( width, height, TextureFormat::F_A8R8G8B8, 1 );

    m_mixChannelsEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_MIX_CHANNELS );
}

// *********************************
//
OutputStreamSharedMem::~OutputStreamSharedMem  ()
{
    delete m_shmRT;
    delete m_shmVideoBuffer;
}

// *********************************
//
void    OutputStreamSharedMem::ProcessFrameData  ( NRenderContext * ctx, RenderResult * input )
{
    if( m_shmRT == nullptr )
    {
        auto w = GetWidth();
        auto h = GetHeight();

        m_shmRT = allocator( ctx )->CreateCustomRenderTarget( w, h, RenderTarget::RTSemantic::S_DRAW_READ );
    }

    // FIXME: nrl - use result to cache resacaled textures between outputs - it is a valid place as it is still a rendering result (e.g. when SD output is exactly the same as SHM output)
    // FIXME: nrl - neither effecto nor readback are necessary in such case

    // FIXME: nrl - this is a bit of an overkill, but let's update it every frame here
    UpdateEffectValues();

    auto rct = GetActiveRenderChannel();
    assert( input->IsActive( rct ) && input->ContainsValidData( rct ) );

    m_activeRenderOutput.SetEntry( 0, input->GetActiveRenderTarget( rct ) );
    m_mixChannelsEffect->Render( ctx, m_shmRT, m_activeRenderOutput );

    renderer( ctx )->ReadColorTexture( 0, m_shmRT, m_shmTexture );

    m_shmVideoBuffer->PushFrame( m_shmTexture );

}

// *********************************
//
void            OutputStreamSharedMem::UpdateEffectValues      ()
{
    auto state = m_mixChannelsEffect->GetState();
    
    auto mappingVal = state->GetValueAt( 0 ); assert( mappingVal->GetName() == "channelMapping" );
    auto maskVal    = state->GetValueAt( 1 ); assert( maskVal->GetName() == "channelMask" );

    auto mapping = GetChannelMapping();
    auto mask = GetChannelMask();

    QueryTypedValue< ValueIntPtr >( mappingVal )->SetValue( mapping );
    QueryTypedValue< ValueVec4Ptr >( maskVal )->SetValue( mask );
}

} //nrl
} //bv


// Algo:
// if w != origW || h != origH || state != rgba
//  mixchannels->Render( output )
//  tex = output->ReadTexture
// else
//  tex = result->ReadTexture
