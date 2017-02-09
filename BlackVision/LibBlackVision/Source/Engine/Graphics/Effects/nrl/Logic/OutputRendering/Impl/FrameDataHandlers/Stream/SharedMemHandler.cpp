#include "stdafx.h"

#include "SharedMemHandler.h"

#include "Engine/Graphics/Effects/nrl/Logic/Components/RenderChannel.h"
#include "Engine/Graphics/Effects/nrl/Logic/Components/NRenderContext.h"

#include "Engine/Graphics/Effects/nrl/Logic/FullscreenRendering/NFullscreenEffectFactory.h"

#include "Engine/Graphics/Effects/nrl/Logic/State/NOutputState.h"


namespace bv { namespace nrl {
   
// **************************
//
SharedMemHandler::SharedMemHandler      ( unsigned int width, unsigned int height )
    : m_width( width )
    , m_height( height )
    , m_mixChannelsEffect( nullptr )
    , m_activeRenderOutput( 1 )
    , m_shmRT( nullptr )
    , m_shmTexture( nullptr )
    , m_shmVideoBuffer( nullptr )
{
    m_shmVideoBuffer = SharedMemoryVideoBuffer::Create( width, height, TextureFormat::F_A8R8G8B8 );

    m_mixChannelsEffect = CreateFullscreenEffect( NFullscreenEffectType::NFET_MIX_CHANNELS );
}

// **************************
//
SharedMemHandler::~SharedMemHandler     ()
{
    delete m_shmRT;

    delete m_mixChannelsEffect;
}

// **************************
//
void                                SharedMemHandler::HandleFrameData     ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * channel )
{
    //1. Prepare memory representation of current frame
    auto shmFrame = PrepareFrame( state, ctx, channel );

    //2. Process memory representation of current frame
    ProcessFrame( shmFrame );
}

// **************************
//
NFullscreenEffectComponentStatePtr  SharedMemHandler::GetInternalFSEState       ()
{
    return m_mixChannelsEffect->GetState();
}

// **************************
//
Texture2DPtr    SharedMemHandler::PrepareFrame                                  ( const NOutputState & state, NRenderContext * ctx, const RenderChannel * inputChannel )
{
    auto inputRenderTarget = inputChannel->GetActiveRenderTarget();  

    Texture2DPtr outputFrame;

    if( m_width == inputRenderTarget->Width() && m_height == inputRenderTarget->Height() && state.RepresentsDefaultTexture() )
    {
        outputFrame = ReadDefaultTexture( ctx, inputChannel );
    }
    else
    {
        outputFrame = ReadMixChannelsTexture( ctx, inputRenderTarget );
    }

    return outputFrame;
}

// **************************
//
void            SharedMemHandler::ProcessFrame                                  ( Texture2DPtr frame )
{
    m_shmVideoBuffer->PushFrame( frame );
}

// **************************
//
Texture2DPtr    SharedMemHandler::ReadDefaultTexture                            ( NRenderContext * ctx, const RenderChannel * inputChannel )
{
	return inputChannel->ReadColorTexture( renderer( ctx ) );
}

// **************************
// FIXME: implement is as a part of some readback cache
Texture2DPtr    SharedMemHandler::ReadMixChannelsTexture                        ( NRenderContext * ctx, const RenderTarget * inputRenderTarget )
{
    if( m_shmRT == nullptr )
    {
        m_shmRT = allocator( ctx )->CreateCustomRenderTarget( m_width, m_height, RenderTarget::RTSemantic::S_DRAW_READ );
    }

    m_activeRenderOutput.SetEntry( 0, inputRenderTarget );
    m_mixChannelsEffect->Render( ctx, m_shmRT, m_activeRenderOutput );

    renderer( ctx )->ReadColorTexture( 0, m_shmRT, m_shmTexture );

    return m_shmTexture;
}

} //nrl
} //bv
