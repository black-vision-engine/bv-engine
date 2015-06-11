#include "RenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Tools/HerarchicalProfiler.h"
#include "Tools/HRTimer.h"

#include "OffscreenRenderLogic.h"
#include "BVConfig.h"

#include "BVGL.h"

//pablito
#define USE_VIDEOCARD	
#include "Log.h"
#include "ConfigManager.h"
#include <boost/lexical_cast.hpp>

#define USE_HACK_FRIEND_NODE_MASK_IMPL

namespace bv {

extern HighResolutionTimer GTimer;

// *********************************
//
RenderLogic::RenderLogic     ()
{
    m_offscreenRenderLogic = new OffscreenRenderLogic( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.NumRedbackBuffersPerRT() );
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
	m_VideoCardManager->m_IsEnding = true;
    delete m_offscreenRenderLogic;
}

// *********************************
//
void    RenderLogic::SetCamera       ( Camera * cam )
{
    m_offscreenRenderLogic->SetRendererCamera( cam );
}

//pablito:
// *********************************
//
void	RenderLogic::SetVideoCardManager(bv::videocards::VideoCardManager* videoCardManager,Renderer * renderer)
{
		m_VideoCardManager = videoCardManager;
		#ifdef USE_VIDEOCARD
			m_VideoCardManager = videoCardManager;
			InitVideoCards( renderer );
		#endif
		
}

// *********************************
//
void RenderLogic::InitVideoCards     ( Renderer * renderer )
{
	if(!DefaultConfig.ReadbackFlag() )
    {
		Log::A("VIDEOCARDS", "INFO", "Config file prevents from initializing VideoCards...");
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
		Log::A("VIDEOCARDS", "ERROR", "No videocards present in system, aborting videocard initialization...");
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

		if(m_VideoCardManager->InitVideoCardManager(m_offscreenRenderLogic->GetHackBuffersUids( renderer ))) 
		m_VideoCardManager->StartVideoCards();


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
void    RenderLogic::RenderFrame     ( Renderer * renderer, SceneNode * node )
{



    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );

    renderer->ClearBuffers();
    renderer->PreDraw();

   
        m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
        renderer->ClearBuffers();
    
    renderer->ClearBuffers();

    if( node )
        RenderNode( renderer, node );

   
        m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
        m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

        m_offscreenRenderLogic->DrawDisplayRenderTarget( renderer );
    

    m_offscreenRenderLogic->DrawDisplayRenderTarget( renderer );

    renderer->PostDraw();
    renderer->DisplayColorBuffer();
}

// *********************************
//
void    RenderLogic::RenderNode      ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        if( node->IsOverridenAM() )
        {
            RenderAlphaMask( renderer, node );
        }
        else if( node->IsOverridenNM() )
        {
#ifndef USE_HACK_FRIEND_NODE_MASK_IMPL
            RenderNodeMask( renderer, node );
 #else
            RenderNodeMask1( renderer, node );
#endif
        }
        else
        {
            RenderVanilla( renderer, node );
        }
    }
}

// *********************************
//
void    RenderLogic::RenderVanilla   ( Renderer * renderer, SceneNode * node )
{
    DrawNode( renderer, node );
}

// *********************************
//
void    RenderLogic::RenderAlphaMask ( Renderer * renderer, SceneNode * node )
{
    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();

    DrawNode( renderer, node );

    m_offscreenRenderLogic->DrawTopAuxRenderTarget( renderer, node->GetOverrideAlphaVal() );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
}

// *********************************
//
void    RenderLogic::RenderNodeMask  ( Renderer * renderer, SceneNode * node )
{
    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();
        
    DrawChildren( renderer, node );

    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();

    DrawNodeOnly( renderer, node );

    m_offscreenRenderLogic->DrawAMTopTwoRenderTargets( renderer, node->GetOverrideAlphaVal() );
    
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
    m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
}

// *********************************
//
void    RenderLogic::RenderNodeMask1 ( Renderer * renderer, SceneNode * node )
{
    if( node->NumChildNodes() < 2 )
    {
        RenderVanilla( renderer, node );
    }
    else
    {
        DrawNodeOnly( renderer, node );

        m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();
        
        // MASK
        RenderNode( renderer, node->GetChild( 1 ) ); 

        m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );
        renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
        renderer->ClearBuffers();

        // FOREGROUND
        RenderNode( renderer, node->GetChild( 0 ) ); 

        m_offscreenRenderLogic->DrawAMTopTwoRenderTargets( renderer, node->GetOverrideAlphaVal() );
    
        m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );
        m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

        m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

        DrawChildren( renderer, node, 2 );
    }
}

// *********************************
//
void    RenderLogic::DrawNode        ( Renderer * renderer, SceneNode * node )
{
    HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor" );
    DrawNodeOnly( renderer, node );

    DrawChildren( renderer, node );
}

// *********************************
//
void    RenderLogic::DrawNodeOnly   ( Renderer * renderer, SceneNode * node )
{
    renderer->Draw( static_cast<bv::RenderableEntity *>( node->GetTransformable() ) );
}

// *********************************
//
void    RenderLogic::DrawChildren   ( Renderer * renderer, SceneNode * node, int firstChildIdx )
{
    for ( unsigned int i = firstChildIdx; i < (unsigned int) node->NumChildNodes(); i++ )
    {
        HPROFILER_SECTION( "RenderNode::RenderNode" );
        RenderNode( renderer, node->GetChild( i ) ); 
    }
}

// *********************************
//
void    RenderLogic::FrameRendered   ( Renderer * renderer )
{
    static int w = 0;
    static int h = 0;

    if( !DefaultConfig.ReadbackFlag() )
    {
        //Not needed as it does not make sense without readback delay
        //m_offscreenRenderLogic->SwapDisplayRenderTargets();
        return;
    }

    if( w != renderer->GetWidth() || h != renderer->GetHeight() )
    {
        w = renderer->GetWidth();
        h = renderer->GetHeight();

        printf( "Framebuffer resolution changed to %dx%d\n", w, h );
    }

    static double totalElapsed = 0.0;
    static int nFrames = 1;
    static int nPasses = 0;
    static int nReadbackFrame = 0;

    double readbackStart = GTimer.CurElapsed();
    auto frame = m_offscreenRenderLogic->ReadDisplayTarget( renderer, nReadbackFrame );
    nReadbackFrame = ( nReadbackFrame + 1 ) % m_offscreenRenderLogic->NumReadBuffersPerRT();
	auto FrameNo = m_offscreenRenderLogic->ReadDisplayTarget( renderer, 0 );

    //GPUDirect;
	if(m_VideoCardManager->m_Enabled)
	{
		if( m_VideoCardManager->m_CurrentTransferMode == bv::videocards::VideoCard_RAM_GPU::GPU )
		{          
			//m_offscreenRenderLogic->TransferFromGPUToSDI( renderer, m_VideoCardManager );
			//m_offscreenRenderLogic->SwapDisplayRenderTargets();
			//todo: fix gpu direct
		}
		else if( m_VideoCardManager->m_CurrentTransferMode==bv::videocards::VideoCard_RAM_GPU::RAM )
		{
			
			m_VideoCardManager->GetBufferFromRenderer(FrameNo);
		}
	}
    double readbackTime = GTimer.CurElapsed() - readbackStart;

    m_offscreenRenderLogic->SwapDisplayRenderTargets();

    totalElapsed += readbackTime;

    //printf( "Time cur %.5f ms of total %.5f ms\n", 1000.f * readbackTime, 1000.f * totalElapsed );

    if( nFrames % 50 == 0 )
    {
        //double avg = totalElapsed / (double) nFrames;

        nPasses++;
        totalElapsed = 0.0;

        if ( nPasses % 3 == 0 )
        {
            nPasses = 0;

            //printf( "Avg readback time from last %d frames took %.4f ms\n", nFrames, avg * 1000 );
        }

        nFrames = 0;
    }

    nFrames++;
}

// *********************************
//
void    RenderLogic::PrintGLStats    (  bool detailed  )
{
    if ( detailed )
    {
        BVGL::PrintCompleteSummary( " ************************* DETAILED GL STATS ********************************** " );
    }
    else
    {
        BVGL::PrintShortSummary( " ************************* SHORT GL STATS ********************************** " );
    }
}

} //bv
