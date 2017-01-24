#include "stdafx.h"

#include "OutputStreamSharedMem.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/OutputRendering/RenderResult.h"
#include "Engine/Graphics/Effects/nrl/Logic/NRenderContext.h"


namespace bv { namespace nrl {

// *********************************
//
OutputStreamSharedMem::OutputStreamSharedMem   ( unsigned int width, unsigned int height )
    : OutputInstance( width, height )
    , m_activeRenderOutput( 1 )
    , m_shmRT( nullptr )
    , m_shmTexture( nullptr )
    , m_shmVideoBuffer( nullptr )
{
    m_shmVideoBuffer = SharedMemoryVideoBuffer::Create( width, height, TextureFormat::F_A8R8G8B8 );

    m_mixChannelsEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_MIX_CHANNELS );
}

// *********************************
//
OutputStreamSharedMem::~OutputStreamSharedMem  ()
{
    delete m_shmRT;
    delete m_mixChannelsEffect;
}

// *********************************
//
void    OutputStreamSharedMem::ProcessFrameData  ( NRenderContext * ctx, RenderResult * input )
{
    //1. Update internal output state
    // FIXME: nrl - this is a bit of an overkill, but let's update it every frame here
    UpdateEffectValues();
    
    //2. Prepare memory representation of current frame
    auto shmFrame = PrepareFrame( ctx, input );

    //3. Process memory representation of current frame
    ProcessFrame( shmFrame );
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

// *********************************
//
Texture2DPtr    OutputStreamSharedMem::PrepareFrame             ( NRenderContext * ctx, RenderResult * input )
{
    auto rct = GetActiveRenderChannel();
    assert( input->IsActive( rct ) && input->ContainsValidData( rct ) );

    auto inputRenderTarget = input->GetActiveRenderTarget( rct );  

    Texture2DPtr outputFrame;

    if( GetWidth() == inputRenderTarget->Width() && GetHeight() == inputRenderTarget->Height() && AccessOutputState().RepresentsDefaultTexture() )
    {
        outputFrame = ReadDefaultTexture( ctx, input, rct );
    }
    else
    {
        outputFrame = ReadMixChannelsTexture( ctx, inputRenderTarget );
    }

    return outputFrame;
}

// *********************************
//
void            OutputStreamSharedMem::ProcessFrame            ( Texture2DPtr frame )
{
    m_shmVideoBuffer->PushFrame( frame );
}

// *********************************
//
Texture2DPtr    OutputStreamSharedMem::ReadDefaultTexture      ( NRenderContext * ctx, RenderResult * input, RenderChannelType rct )
{
	return input->ReadColorTexture( renderer( ctx ), rct );
}

// *********************************
//
Texture2DPtr    OutputStreamSharedMem::ReadMixChannelsTexture  ( NRenderContext * ctx, const RenderTarget * inputRenderTarget )
{
    if( m_shmRT == nullptr )
    {
        auto w = GetWidth();
        auto h = GetHeight();

        m_shmRT = allocator( ctx )->CreateCustomRenderTarget( w, h, RenderTarget::RTSemantic::S_DRAW_READ );
    }

    m_activeRenderOutput.SetEntry( 0, inputRenderTarget );
    m_mixChannelsEffect->Render( ctx, m_shmRT, m_activeRenderOutput );

    renderer( ctx )->ReadColorTexture( 0, m_shmRT, m_shmTexture );

    return m_shmTexture;
}

} //nrl
} //bv
