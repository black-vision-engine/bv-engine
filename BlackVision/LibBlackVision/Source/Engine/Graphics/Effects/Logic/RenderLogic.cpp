#include "stdafx.h"

#include "RenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Engine/Graphics/Effects/Utils/RenderLogicContext.h"
#include "Engine/Graphics/Effects/Utils/OffscreenDisplay.h"

#include "Engine/Graphics/Effects/Logic/VideoOutputRendering/VideoOutputRenderLogic.h"
#include "Engine/Graphics/Effects/Fullscreen/FullscreenEffectFactory.h"

#include "Engine/Graphics/Shaders/RenderablePass.h"
#include "Engine/Graphics/SceneGraph/RenderableEntityWithBoundingBox.h"
#include "Engine/Graphics/Effects/BoundingBoxEffect.h"

#include "Engine/Graphics/Rendering/Logic/OfflineRendering/ScreenShotLogic.h"

#include "Tools/Profiler/HerarchicalProfiler.h"
//#include "FrameStatsService.h"

#include "UseLoggerLibBlackVision.h"

#include "LibImage.h"

//pablito
#define USE_VIDEOCARD	
#include "ConfigManager.h"
#include <boost/lexical_cast.hpp>


namespace bv {

// *********************************
//DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.ReadbackFlag(), DefaultConfig.DisplayVideoCardOutput()
RenderLogic::RenderLogic     ( unsigned int width, unsigned int height, const glm::vec4 & clearColor, bool useReadback, bool useVideoCardOutput, bool renderToSharedMemory )
    : m_rtStackAllocator( width, height, TextureFormat::F_A8R8G8B8 )
    , m_blitEffect( nullptr )
    , m_videoOutputRenderLogic( nullptr )
    , m_ctx( nullptr )
    , m_clearColor( clearColor )
    , m_renderToSharedMemory( renderToSharedMemory )
{
    auto videoCardEnabled   = useReadback;
    auto previewAsVideoCard = useVideoCardOutput;

    unsigned int numFrameRenderTargets = videoCardEnabled || previewAsVideoCard ? 2 : 1;

    m_blitEffect                = CreateFullscreenEffectInstance( FullscreenEffectType::FET_SIMPLE_BLIT );
    m_offscreenDisplay          = new OffscreenDisplay( &m_rtStackAllocator, numFrameRenderTargets, videoCardEnabled || previewAsVideoCard );
    m_videoOutputRenderLogic    = new VideoOutputRenderLogic( height ); // FIXME: interlace odd/even setup
    m_SharedMemoryVideoBuffer	= new SharedMemoryVideoBuffer();

    m_displayVideoCardPreview   = previewAsVideoCard;
    m_useVideoCardOutput        = videoCardEnabled;

    m_screenShotLogic           = new ScreenShotLogic( 1 );

    auto effect = new BoundingBoxEffect(); // FIXME: memory leak
    m_boundingBoxEffect         = effect->GetPass( 0 );
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
    delete m_offscreenDisplay;
    delete m_blitEffect;
    delete m_screenShotLogic;
    delete m_ctx;
}

// *********************************
//
void    RenderLogic::RenderFrame    ( Renderer * renderer, SceneNode * sceneRoot )
{
    renderer->PreDraw();

    RenderFrameImpl( renderer, sceneRoot );
    
    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

////pablito:
//// *********************************
////
//void	RenderLogic::SetVideoCardManager( bv::videocards::VideoCardManager* videoCardManager )
//{
//		m_VideoCardManager = videoCardManager;
//		#ifdef USE_VIDEOCARD
//			m_VideoCardManager = videoCardManager;
//
//			InitVideoCards();
//		#endif
//		
//}
//
//// *********************************
////
//void RenderLogic::InitVideoCards     ()
//{
//	if( !m_useVideoCardOutput )
//    {
//		LOG_MESSAGE( SeverityLevel::info ) << "Config file prevents from initializing VideoCards...";
//		return;
//	}
//    
//    m_VideoCardManager->m_VideoCardConfig.ReadbackFlag = m_useVideoCardOutput;
//    
//    m_VideoCardManager->m_VideoCardConfig.BlueFish = bv::ConfigManager::GetBool("VideoCards/BlueFish");
//    m_VideoCardManager->m_VideoCardConfig.BlackMagic = bv::ConfigManager::GetBool("VideoCards/BlackMagic");
//    m_VideoCardManager->m_VideoCardConfig.superMagic = bv::ConfigManager::GetBool("VideoCards/BlackMagic/SuperMagic");
//    m_VideoCardManager->m_VideoCardConfig.resolutionOld = bv::ConfigManager::GetString("Resolution");
//    m_VideoCardManager->m_VideoCardConfig.transferMode = bv::ConfigManager::GetString("TransferMode");
//
//    m_VideoCardManager->ReadConfig();
//    
//	if(m_VideoCardManager->GetVideoCardsSize()==0)
//	{
//		LOG_MESSAGE( SeverityLevel::error ) << "No videocards present in system, aborting videocard initialization...";
//	}else{
//		unsigned int bln = 0;
//		for(unsigned int i = 0   ;   i < m_VideoCardManager->GetVideoCardsSize() ; i++)
//		{
//			if( m_VideoCardManager->GetVideoCard(i)->GetBrand() == "BlueFish" )
//			{
//				bln++;
//				m_VideoCardManager->m_VideoCardConfig.blueFishCount = bv::ConfigManager::GetInt("VideoCards/BlueFish/BlueFishAmount");
//				if( i > m_VideoCardManager->m_VideoCardConfig.blueFishCount) continue;
//
//				m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig.push_back(bv::videocards::VideoCardConfig());
//
//				m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount = bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/ChannelAmount");
//
//            
//				if(m_VideoCardManager->GetVideoCard(i)->DetectOutputs() < m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount)
//				{
//					printf("VideoCards","ERROR", "Too many Channels to configure");                
//					return;
//				}
//
//				for(unsigned int i = 0;  i < m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount; i++)
//				{
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector.push_back(bv::videocards::ChannelConfig());
//				}
//
//            
//				for(unsigned int z = 0; z < m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount;z++)
//				{
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].name = bv::ConfigManager::GetString("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z));
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.type =  bv::ConfigManager::GetString("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Type");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].renderer = (unsigned short)bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Renderer");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.resolution =  (unsigned short)bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Resolution");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refresh = (unsigned short)bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Refresh");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.interlaced =  bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Interlaced");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.flipped = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Flipped");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].playback = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].capture = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Input");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].inputConfig.playthrough = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Input/Playthrough");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].inputConfig.type = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Input/Type");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.referenceMode = bv::ConfigManager::GetString("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Reference");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refH = bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Reference/H");
//					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refV =  bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Reference/V");
//																	 
//				}
//			}
//			else if(m_VideoCardManager->GetVideoCard(i)->GetBrand() == "BlackMagic")
//			{
//				//m_VideoCardManager->GetVideoCard(i)->SetReferenceModeValue(bv::ConfigManager::GetString("VideoCards/BlackMagic/Reference"));
//				//m_VideoCardManager->m_VideoCardConfig.m_BlackMagicConfig[0].channelConfigVector[0].m_outputConfig.referenceMode = bv::ConfigManager::GetString("VideoCards/BlackMagic/Reference");
//				//m_VideoCardManager->GetVideoCard(i)->SetReferenceOffsetValue(bv::ConfigManager::GetInt("VideoCards/BlackMagic/Reference/H"),bv::ConfigManager::GetInt("VideoCards/BlackMagic/Reference/V"));
//			}
//		}
//
//        // m_VideoCardManager->StartVideoCards();
//		if(m_VideoCardManager->InitVideoCardManager(std::vector<int>()))   // FIXME: default->TDP2015
//		{
//			m_VideoCardManager->StartVideoCards();
//		}
//	}
//
//
//
//
//
//    //vc
//	/*if(ConfigManager::GetBool("SuperMagic"))
//    {
//        m_VideoCardManager->SuperMagic=true;
//    }
//    if(ConfigManager::GetString("Resolution")=="SD")
//    {
//        m_VideoCardManager->CurrentDislpayMode =  VideoCard_Modes::SD;
//    }
//
//    if(ConfigManager::GetString("TransferMode")=="RAM")
//    {
//        m_VideoCardManager->CurrentTransferMode =  VideoCard_RAM_GPU::RAM;
//    }
//    else if(ConfigManager::GetString("TransferMode")=="GPU")
//    {
//        m_VideoCardManager->CurrentTransferMode =  VideoCard_RAM_GPU::GPU;
//    }
//
//    if( !DefaultConfig.ReadbackFlag() )
//    {
//        return;
//    }
//        
//    if(m_VideoCardManager->CurrentTransferMode != GPU )
//    {
//        for(int i = 0; i < 4; i++ )
//        {
//            m_VideoCardManager->Midgard->Buffers[i] = m_offscreenRenderLogic->ReadDisplayTarget( renderer, i );
//            m_offscreenRenderLogic->SwapDisplayRenderTargets();
//            m_VideoCardManager->Midgard->Buffers[i] = m_offscreenRenderLogic->ReadDisplayTarget( renderer, i );
//        }
//    }
//
//    
//    m_VideoCardManager->InitVideoCards( m_offscreenRenderLogic->GetHackBuffersUids( renderer ) );
//    
//	Log::A("VideoCards","INFO","Detected " + to_string(m_VideoCardManager->VideoCards.size()) + " videocard(s)");
//
//    for(unsigned int i = 0 ;   i < m_VideoCardManager->VideoCards.size()   ;   i++)
//    {
//        m_VideoCardManager->ActivateVideoCard(m_VideoCardManager->VideoCards[i]);
//        //todo: unhak me
//        break;
//    }
//    
//	if(m_VideoCardManager->CurrentTransferMode != GPU)
//    { 
//        m_VideoCardManager->StartPlayback();
//    }
//	*/
//
//
//}
// *********************************
//
void    RenderLogic::RenderFrameImpl ( Renderer * renderer, SceneNode * sceneRoot )
{
    auto rt = m_offscreenDisplay->GetCurrentFrameRenderTarget();

    RenderRootNode( renderer, sceneRoot, rt );
    {
        HPROFILER_SECTION( "PreFrame Setup", PROFILER_THREAD1 );
    }

    FrameRendered( renderer );

    UpdateOffscreenState();
}

// *********************************
//
//  if not DisplayAsVideoOutput:
//      BlitToWindow()
//  else:
//      GPURenderPreVideo()
//      BlitToWindow()
//
//      if PushToVideoCard:
//          Readback()
//          Push()
//
void    RenderLogic::FrameRendered   ( Renderer * renderer )
{
    auto prevRt = m_offscreenDisplay->GetCurrentFrameRenderTarget();
    auto ctx = GetContext( renderer );


    if( m_screenShotLogic->ReadbackNeeded() )
    {
        auto rt = m_offscreenDisplay->GetCurrentFrameRenderTarget();
        m_screenShotLogic->FrameRendered( rt, ctx );
    }


    if( m_displayVideoCardPreview )
    {
        auto videoRt    = m_offscreenDisplay->GetVideoRenderTarget          ();
        auto curFrameRt = m_offscreenDisplay->GetCurrentFrameRenderTarget   ();
        auto prvFrameRt = m_offscreenDisplay->GetPreviousFrameRenderTarget  ();

        m_videoOutputRenderLogic->Render( videoRt, curFrameRt, prvFrameRt, ctx );

        prevRt = videoRt;
    }

    BlitToPreview( prevRt, ctx );


    if( m_useVideoCardOutput )
    {
        OnVideoFrameRendered( ctx );
    }
}


//// *********************************
////
//void    RenderLogic::PushToVideoCard  ( Texture2DConstPtr frame ) // FIXME: pablito source code.
//{
//    //GPUDirect;
//	if( m_VideoCardManager->IsEnabled() )
//	{
//		if( m_VideoCardManager->m_CurrentTransferMode == bv::videocards::VideoCard_RAM_GPU::GPU )
//		{          
//			//m_offscreenRenderLogic->TransferFromGPUToSDI( renderer, m_VideoCardManager );
//			//m_offscreenRenderLogic->SwapDisplayRenderTargets();
//			//todo: fix gpu direct
//		}
//		else if( m_VideoCardManager->m_CurrentTransferMode==bv::videocards::VideoCard_RAM_GPU::RAM )
//		{
//			m_VideoCardManager->GetBufferFromRenderer( frame );
//
//			
//
//		}
//	}
//	
//	if( m_renderToSharedMemory )
//		m_SharedMemoryVideoBuffer->DistributeFrame(frame);
//}

// *********************************
//
void    RenderLogic::RenderRootNode  ( Renderer * renderer, SceneNode * sceneRoot, RenderTarget * rt )
{
    //FIXME: assumes only one renderer instance per application
    auto ctx = GetContext( renderer );

    assert( renderer == ctx->GetRenderer() );

    // FIXME: verify that all rendering paths work as expected
    if( sceneRoot )
    {
        enable( ctx, rt );
        
        clearBoundRT( ctx, m_clearColor );

        RenderNode( sceneRoot, ctx );

        disableBoundRT( ctx );
    }
}

namespace {

bool IsSelected( SceneNode * node ) 
{ 
    return node->IsSelected();
}

glm::vec4 BoundingBoxColor( SceneNode * node )
{
    return node->GetBoundingBoxColor();
}

} // anonymous

// *********************************
//
void    RenderLogic::RenderNode      ( SceneNode * node, RenderLogicContext * ctx )
{
    if ( node->IsVisible() )
    {
        auto effect = node->GetNodeEffect();

        if( !effect || effect->GetType() ==  NodeEffectType::NET_DEFAULT )
        {
            // Default render logic
            DrawNode( node, ctx );
        }
        else
        {
            effect->Render( node, ctx );
        }

        if( IsSelected( node ) )
            RenderBoundingBox( node, ctx, BoundingBoxColor( node ) );
    }
}

// *********************************
//
void    RenderLogic::DrawNode        ( SceneNode * node, RenderLogicContext * ctx )
{
    HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
    DrawNodeOnly( renderer( ctx ), node );

    RenderChildren( node, ctx );
}

// *********************************
//
void    RenderLogic::DrawNodeOnly    ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    RenderLogic::RenderChildren  ( SceneNode * node, RenderLogicContext * ctx, int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNode  ( node->GetChild( i ), ctx ); 
    }
}

namespace {

RenderableEntity * GetBoundingBox( SceneNode * node )
{
    auto obj = Cast< RenderableEntityWithBoundingBox * >( node->GetTransformable() );
    assert( obj );
    auto bb = obj->GetBoundingBox();
    if( bb )
        return Cast< RenderableEntity * >( bb );
    else
        return nullptr;
}

}

// ***********************
//
void    RenderLogic::RenderBoundingBox( SceneNode * node, RenderLogicContext * ctx, glm::vec4 color )
{
    auto * bb = GetBoundingBox( node );

    if( bb )
    {
        auto renderer = ctx->GetRenderer();

        auto param = Cast< ShaderParamVec4 * >( m_boundingBoxEffect->GetPixelShader()->GetParameters()->AccessParam( "color" ) );
        param->SetValue( color );

        renderer->Enable( m_boundingBoxEffect, bb );
        renderer->DrawRenderable( bb );
    }
}

// *********************************
//
RenderLogicContext *    RenderLogic::GetContext         ( Renderer * renderer )
{
    if( !m_ctx )
    {
        m_ctx = new RenderLogicContext( renderer, &m_rtStackAllocator, this );
    }

    return m_ctx;
}

// *********************************
//
void                    RenderLogic::BlitToPreview          ( RenderTarget * rt, RenderLogicContext * ctx )
{
    if( m_blitEffect->GetRenderTarget( 0 ) != rt )
    {
        m_blitEffect->SetRenderTarget( 0, rt );
    }

    // Clear 'preview' render target.
    clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.f ) );

    m_blitEffect->Render( rt, ctx );
}

// *********************************
//
void                    RenderLogic::UpdateOffscreenState   ()
{
    m_offscreenDisplay->UpdateActiveRenderTargetIdx();
    m_offscreenDisplay->UpdateVideoRenderTargetIdx();
}

// *********************************
//
void                    RenderLogic::OnVideoFrameRendered   ( RenderLogicContext * ctx )
{
    //static Texture2DPtr vtx[] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    //const static unsigned int numTextures = 7;
    //static unsigned int i = 0;

    //auto vrt = m_offscreenDisplay->GetVideoRenderTarget          ();

    //renderer->ReadColorTexture( 0, vrt, vtx[ i % numTextures ] );

    //auto name = std::string( "e:/grabs/frame" ) + std::to_string( 10000 + i ) + ".bmp";
    //{ name; }
    //// image::SaveBMPImage( name, vtx[ i % numTextures ]->GetData(), 1920, 1080, 32 );
    //
 //   PushToVideoCard( vtx[ i % numTextures ] );	
 //   ++i;
    
    auto rt = m_offscreenDisplay->GetVideoRenderTarget();

    m_videoOutputRenderLogic->VideoFrameRendered( rt, ctx );
}

// *********************************
//
VideoOutputRenderLogic *    RenderLogic::GedVideoOutputRenderLogic  ()
{
    return m_videoOutputRenderLogic;
}

// *********************************
//
void                        RenderLogic::MakeScreenShot  ( const std::string& path, unsigned int numFrames, bool onRenderedEvent, bool asyncWrite )
{
    m_screenShotLogic->MakeScreenShot( path, numFrames, onRenderedEvent, asyncWrite );
}

} //bv

/* 

Perfect world:

Rendering -> Frame rendered to texture -> POST RENDER DISPLAY LOGIC

POST RENDER DISPLAY LOGIC: (option a: display to the preview) -> stack current frame -> apply per pixel effect -> interlace with previous frame -> (option b: display to the preview) | display to video card


Not so ideal world:

Rendering -> render logic (with swapping render targets) -> some code dependent on render logic and render targets -> bookkeeping of rendered frames -> not so well isolated POST RENDER DISPLAY LOGIC

POST RENDER DISPLAY LOGIC: (option a: display to the preview) -> somehow use render logic and offscreen render logic to interlace last two frames -> apply pixel shaders afterwards -> (option b: display to the preview) | display to video card

What makes it even more cumbersome is that RenderLogic is used by global effect render logic inplementation whereas a separated class created solely for this purpose should be used
RenderLogic should only serve as a dispatcher of rendering tasks to appropriate render logic implementations
OffscreenRenderLogic should also be split in two - blending logic and other logic AND a separate class used only to manipulate render targets

*/
