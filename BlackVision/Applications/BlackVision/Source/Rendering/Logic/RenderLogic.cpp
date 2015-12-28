#include "RenderLogic.h"

#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Rendering/Utils/RenderLogicContext.h"
#include "Rendering/Utils/OffscreenDisplay.h"

#include "Rendering/Logic/FrameRendering/NodeEffect/NodeEffectRenderLogic.h"
#include "Rendering/Logic/FullScreen/Impl/BlitFullscreenEffect.h"
#include "Rendering/Logic/VideoOutputRendering/VideoOutputRenderLogic.h"
#include "Rendering/Logic/OfflineRendering/ScreenShotLogic.h"

#include "Tools/Profiler/HerarchicalProfiler.h"
#include "FrameStatsService.h"

#include "UseLoggerBVAppModule.h"

#include "BVConfig.h"

//pablito
#define USE_VIDEOCARD	
#include "ConfigManager.h"
#include <boost/lexical_cast.hpp>

namespace bv {

// *********************************
//
RenderLogic::RenderLogic     ()
    : m_rtStackAllocator( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), TextureFormat::F_A8R8G8B8 )
    , m_blitEffect( nullptr )
    , m_videoOutputRenderLogic( nullptr )
{
    auto videoCardEnabled   = DefaultConfig.ReadbackFlag();
    auto previewAsVideoCard = DefaultConfig.DisplayVideoCardOutput();

    unsigned int numFrameRenderTargets = videoCardEnabled || previewAsVideoCard ? 2 : 1;

    m_offscreenDisplay          = new OffscreenDisplay( &m_rtStackAllocator, numFrameRenderTargets, videoCardEnabled || previewAsVideoCard );
    m_videoOutputRenderLogic    = new VideoOutputRenderLogic( DefaultConfig.DefaultHeight() ); // FIXME: interlace odd/even setup

    m_displayVideoCardPreview   = previewAsVideoCard;
    m_useVideoCardOutput        = videoCardEnabled;
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
    delete m_offscreenDisplay;
    delete m_blitEffect;
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

//pablito:
// *********************************
//
void	RenderLogic::SetVideoCardManager( bv::videocards::VideoCardManager* videoCardManager )
{
		m_VideoCardManager = videoCardManager;
		#ifdef USE_VIDEOCARD
			m_VideoCardManager = videoCardManager;
			InitVideoCards();
		#endif
		
}

// *********************************
//
void RenderLogic::InitVideoCards     ()
{
	if(!DefaultConfig.ReadbackFlag() )
    {
		LOG_MESSAGE( SeverityLevel::info ) << "Config file prevents from initializing VideoCards...";
		return;
	}
    m_VideoCardManager->m_VideoCardConfig.ReadbackFlag = bv::DefaultConfig.ReadbackFlag();
    m_VideoCardManager->m_VideoCardConfig.BlueFish = bv::ConfigManager::GetBool("VideoCards/BlueFish");
    m_VideoCardManager->m_VideoCardConfig.BlackMagic = bv::ConfigManager::GetBool("VideoCards/BlackMagic");
    m_VideoCardManager->m_VideoCardConfig.superMagic = bv::ConfigManager::GetBool("VideoCards/BlackMagic/SuperMagic");
    m_VideoCardManager->m_VideoCardConfig.resolutionOld = bv::ConfigManager::GetString("Resolution");
    m_VideoCardManager->m_VideoCardConfig.transferMode = bv::ConfigManager::GetString("TransferMode");
	
    
    m_VideoCardManager->ReadConfig();
    
	if(m_VideoCardManager->GetVideoCardsSize()==0)
	{
		LOG_MESSAGE( SeverityLevel::error ) << "No videocards present in system, aborting videocard initialization...";
	}else{
		unsigned int bln = 0;
		for(unsigned int i = 0   ;   i < m_VideoCardManager->GetVideoCardsSize() ; i++)
		{
			if( m_VideoCardManager->GetVideoCard(i)->GetBrand() == "BlueFish" )
			{
				bln++;
				m_VideoCardManager->m_VideoCardConfig.blueFishCount = bv::ConfigManager::GetInt("VideoCards/BlueFish/BlueFishAmount");
				if( i > m_VideoCardManager->m_VideoCardConfig.blueFishCount) continue;

				m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig.push_back(bv::videocards::VideoCardConfig());

				m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount = bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/ChannelAmount");

            
				if(m_VideoCardManager->GetVideoCard(i)->DetectOutputs() < m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount)
				{
					printf("VideoCards","ERROR", "Too many Channels to configure");                
					return;
				}

				for(unsigned int i = 0;  i < m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount; i++)
				{
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector.push_back(bv::videocards::ChannelConfig());
				}

            
				for(unsigned int z = 0; z < m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelCount;z++)
				{
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].name = bv::ConfigManager::GetString("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z));
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.type =  bv::ConfigManager::GetString("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Type");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].renderer = (unsigned short)bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Renderer");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.resolution =  (unsigned short)bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Resolution");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refresh = (unsigned short)bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Refresh");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.interlaced =  bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Interlaced");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.flipped = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Flipped");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].playback = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].capture = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Input");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].inputConfig.playthrough = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Input/Playthrough");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].inputConfig.type = bv::ConfigManager::GetBool("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Input/Type");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.referenceMode = bv::ConfigManager::GetString("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Reference");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refH = bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Reference/H");
					m_VideoCardManager->m_VideoCardConfig.m_BlueFishConfig[bln-1].channelConfigVector[z].m_outputConfig.refV =  bv::ConfigManager::GetInt("VideoCards/BlueFish/"+boost::lexical_cast<std::string>(bln)+"/Channel"+ boost::lexical_cast<std::string>(z)+"/Output/Reference/V");
																	 
				}
			}
			else if(m_VideoCardManager->GetVideoCard(i)->GetBrand() == "BlackMagic")
			{
				//m_VideoCardManager->GetVideoCard(i)->SetReferenceModeValue(bv::ConfigManager::GetString("VideoCards/BlackMagic/Reference"));
				//m_VideoCardManager->m_VideoCardConfig.m_BlackMagicConfig[0].channelConfigVector[0].m_outputConfig.referenceMode = bv::ConfigManager::GetString("VideoCards/BlackMagic/Reference");
				//m_VideoCardManager->GetVideoCard(i)->SetReferenceOffsetValue(bv::ConfigManager::GetInt("VideoCards/BlackMagic/Reference/H"),bv::ConfigManager::GetInt("VideoCards/BlackMagic/Reference/V"));
			}
		}

        m_VideoCardManager->StartVideoCards();
		//if(m_VideoCardManager->InitVideoCardManager(m_offscreenRenderLogic->GetHackBuffersUids( renderer )))   // FIXME: default->TDP2015
		//    m_VideoCardManager->StartVideoCards();


	}





    //vc
	/*if(ConfigManager::GetBool("SuperMagic"))
    {
        m_VideoCardManager->SuperMagic=true;
    }
    if(ConfigManager::GetString("Resolution")=="SD")
    {
        m_VideoCardManager->CurrentDislpayMode =  VideoCard_Modes::SD;
    }

    if(ConfigManager::GetString("TransferMode")=="RAM")
    {
        m_VideoCardManager->CurrentTransferMode =  VideoCard_RAM_GPU::RAM;
    }
    else if(ConfigManager::GetString("TransferMode")=="GPU")
    {
        m_VideoCardManager->CurrentTransferMode =  VideoCard_RAM_GPU::GPU;
    }

    if( !DefaultConfig.ReadbackFlag() )
    {
        return;
    }
        
    if(m_VideoCardManager->CurrentTransferMode != GPU )
    {
        for(int i = 0; i < 4; i++ )
        {
            m_VideoCardManager->Midgard->Buffers[i] = m_offscreenRenderLogic->ReadDisplayTarget( renderer, i );
            m_offscreenRenderLogic->SwapDisplayRenderTargets();
            m_VideoCardManager->Midgard->Buffers[i] = m_offscreenRenderLogic->ReadDisplayTarget( renderer, i );
        }
    }

    
    m_VideoCardManager->InitVideoCards( m_offscreenRenderLogic->GetHackBuffersUids( renderer ) );
    
	Log::A("VideoCards","INFO","Detected " + to_string(m_VideoCardManager->VideoCards.size()) + " videocard(s)");

    for(unsigned int i = 0 ;   i < m_VideoCardManager->VideoCards.size()   ;   i++)
    {
        m_VideoCardManager->ActivateVideoCard(m_VideoCardManager->VideoCards[i]);
        //todo: unhak me
        break;
    }
    
	if(m_VideoCardManager->CurrentTransferMode != GPU)
    { 
        m_VideoCardManager->StartPlayback();
    }
	*/


}
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

    if( m_displayVideoCardPreview )
    {
        auto videoRt    = m_offscreenDisplay->GetVideoRenderTarget          ();
        auto curFrameRt = m_offscreenDisplay->GetCurrentFrameRenderTarget   ();
        auto prvFrameRt = m_offscreenDisplay->GetPreviousFrameRenderTarget  ();

        m_videoOutputRenderLogic->Render( renderer, videoRt, curFrameRt, prvFrameRt );

        prevRt = videoRt;
    }
   
    BlitToPreview( renderer, prevRt );

    if( m_useVideoCardOutput )
    {
        auto videoRt    = m_offscreenDisplay->GetVideoRenderTarget          ();

        PushToVideoCard( videoRt->ColorTexture( 0 ) );
        //FIXME: VIDEO CART CODE (PUSH FRAME) to be placed here
        OnVideoFrameRendered( renderer );
    }
}


// *********************************
//
void    RenderLogic::PushToVideoCard  ( Texture2DConstPtr frame ) // FIXME: pablito source code.
{
    //GPUDirect;
	if(m_VideoCardManager->IsEnabled())
	{
		if( m_VideoCardManager->m_CurrentTransferMode == bv::videocards::VideoCard_RAM_GPU::GPU )
		{          
			//m_offscreenRenderLogic->TransferFromGPUToSDI( renderer, m_VideoCardManager );
			//m_offscreenRenderLogic->SwapDisplayRenderTargets();
			//todo: fix gpu direct
		}
		else if( m_VideoCardManager->m_CurrentTransferMode==bv::videocards::VideoCard_RAM_GPU::RAM )
		{
			m_VideoCardManager->GetBufferFromRenderer( frame );
		}
	}
}

// *********************************
//
void    RenderLogic::RenderRootNode  ( Renderer * renderer, SceneNode * sceneRoot, RenderTarget * rt )
{
    //FIXME: assumes only one renderer instance per application
    static RenderLogicContext ctx_( renderer, &m_rtStackAllocator, this );
    static RenderLogicContext * ctx = &ctx_;

    assert( renderer == ctx_.GetRenderer() );

    // FIXME: verify that all rendering paths work as expected
	if( sceneRoot )
    {
        enable( ctx, rt );

        clearBoundRT( ctx, glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

        RenderNode( renderer, sceneRoot, ctx );

        disableBoundRT( ctx );
    }
}

// *********************************
//
void    RenderLogic::RenderNode      ( Renderer * renderer, SceneNode * node, RenderLogicContext * ctx )
{
    if ( node->IsVisible() )
    {
        if( node->GetNodeEffect()->GetType() == NodeEffect::Type::T_DEFAULT )
        {
            // Default render logic
            DrawNode( renderer, node, ctx );
        }
        else
        {
            auto effectRenderLogic = m_nodeEffectRenderLogicSelector.GetNodeEffectRenderLogic( node );
               
            effectRenderLogic->RenderNode( node, ctx );
        }
    }
}

// *********************************
//
void    RenderLogic::DrawNode        ( Renderer * renderer, SceneNode * node, RenderLogicContext * ctx )
{
	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
    DrawNodeOnly( renderer, node );

    RenderChildren( renderer, node, ctx );
}

// *********************************
//
void    RenderLogic::DrawNodeOnly    ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    RenderLogic::RenderChildren  ( Renderer * renderer, SceneNode * node, RenderLogicContext * ctx, int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNode  ( renderer, node->GetChild( i ), ctx ); 
    }
}

// *********************************
//
BlitFullscreenEffect *  RenderLogic::AccessBlitEffect   ( RenderTarget * rt )
{
    // FIXME: Blit current render target - suxx a bit - there should be a separate initialization step
    if ( !m_blitEffect )
    {
        auto rtTex = rt->ColorTexture( 0 );

        m_blitEffect = new BlitFullscreenEffect( rtTex, false );
    }

    return m_blitEffect;
}

// *********************************
//
void                    RenderLogic::BlitToPreview      ( Renderer * renderer, RenderTarget * rt )
{
    auto blitter = AccessBlitEffect( rt );

    blitter->Render( renderer );
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
void                    RenderLogic::OnVideoFrameRendered   ( Renderer * renderer )
{
    //FIXME: VIDEO CART CODE (PUSH FRAME) to be placed here
    static int i = 0;

    static Texture2DPtr tx[] = {nullptr, nullptr};

    auto rt = m_offscreenDisplay->GetVideoRenderTarget();

    auto name = std::string( "frame" ) + std::to_string( 10000 + i ) + ".bmp";

    // m_videoOutputRenderLogic->Get
    //renderer->ReadColorTexture( 0, rt, tx[ i % 2 ] );
    //image::SaveBMPImage( name, tx[ i % 2 ]->GetData(), 1920, 1080, 32 );
    /*
    renderer->ReadColorTexture( 0, rt, tx[ 0 ] );
    image::SaveBMPImage( name, tx[ 0 ]->GetData(), 1920, 1080, 32 );
    */
    { renderer; rt; }
    ++i;
}

// *********************************
//
VideoOutputRenderLogic *    RenderLogic::GedVideoOutputRenderLogic  ()
{
    return m_videoOutputRenderLogic;
}

// *********************************
//
void                        RenderLogic::MakeScreenShot  ( const std::string& path, unsigned int numFrames )
{
    m_screenShotLogic->MakeScreenShot( path, numFrames );
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
