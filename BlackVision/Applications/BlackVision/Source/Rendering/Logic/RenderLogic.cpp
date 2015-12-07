#include "RenderLogic.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/SceneGraph/SceneNode.h"
#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Tools/Profiler/HerarchicalProfiler.h"
#include "FrameStatsService.h"

#include "Rendering/Logic/OfflineRendering/ScreenShotLogic.h"
#include "Rendering/OffscreenRenderLogic.h"
#include "BVConfig.h"

#include "BVGL.h"

#include "UseLogger.h"

//pablito
#define USE_VIDEOCARD	
#include "ConfigManager.h"
#include <boost/lexical_cast.hpp>
#include "Rendering/Logic/NodeEffectRendering/NodeEffectRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/DefaultEffectRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/AlphaMaskRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/NodeMaskRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/WireframeRenderLogic.h"
#include "Rendering/Logic/NodeEffectRendering/LightScatteringRenderLogic.h"

#include "Rendering/Logic/VideoOutputRendering/DefaultVideoOutputRenderLogic.h"


namespace bv {

// *********************************
//
RenderLogic::RenderLogic     ()
{
    m_offscreenRenderLogic = new OffscreenRenderLogic( DefaultConfig.DefaultWidth(), DefaultConfig.DefaultHeight(), DefaultConfig.NumRedbackBuffersPerRT() );
    m_videoOutputRenderLogic = new DefaultVideoOutputRenderLogic( DefaultConfig.ReadbackFlag(), DefaultConfig.DisplayVideoCardOutput() );
    m_screenShotLogic = new ScreenShotLogic( m_offscreenRenderLogic->NumReadBuffersPerRT() );

    m_customNodeRenderLogic.push_back( new DefaultEffectRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new AlphaMaskRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new NodeMaskRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new WireframeRenderLogic( this, m_offscreenRenderLogic ) );
    m_customNodeRenderLogic.push_back( new LightScatteringRenderLogic( this, m_offscreenRenderLogic ) );
}

// *********************************
//
RenderLogic::~RenderLogic    ()
{
	m_VideoCardManager->m_IsEnding = true;
    for ( auto rl : m_customNodeRenderLogic )
        delete rl;

    delete m_offscreenRenderLogic;
    delete m_videoOutputRenderLogic;
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
    {
        HPROFILER_SECTION( "PreFrame Setup", PROFILER_THREAD1 );
        PreFrameSetup( renderer );
    }

    {
        HPROFILER_SECTION( "RenderNode", PROFILER_THREAD1 );
        // FIXME: verify that all rendering paths work as expected
	    if( node )
		    RenderNode( renderer, node );
    }

    {
        HPROFILER_SECTION( "PostFrame Setup", PROFILER_THREAD1 );
        PostFrameSetup( renderer );
    }
}

// *********************************
//
void    RenderLogic::PreFrameSetup  ( Renderer * renderer )
{
    renderer->SetClearColor( glm::vec4( 0.f, 0.f, 0.f, 0.0f ) );
    renderer->ClearBuffers();
    renderer->PreDraw();

    m_offscreenRenderLogic->AllocateNewRenderTarget( renderer );
    m_offscreenRenderLogic->EnableTopRenderTarget( renderer );

    renderer->ClearBuffers();
}

// *********************************
//
void    RenderLogic::PostFrameSetup ( Renderer * renderer )
{
    {
        HPROFILER_SECTION( "Video Output Logic", PROFILER_THREAD1 );
        m_offscreenRenderLogic->DisableTopRenderTarget( renderer );
        m_offscreenRenderLogic->DiscardCurrentRenderTarget( renderer );

        m_videoOutputRenderLogic->FrameRenderedNewImpl( renderer, m_offscreenRenderLogic, m_VideoCardManager );
        //m_screenShotLogic->FrameRendered( renderer, m_offscreenRenderLogic );
    }

    {
        HPROFILER_SECTION( "Display Color Buffer", PROFILER_THREAD1 );
        renderer->PostDraw();
        renderer->DisplayColorBuffer();
    }
}

// *********************************
//
void    RenderLogic::RenderNode     ( Renderer * renderer, SceneNode * node )
{
    if ( node->IsVisible() )
    {
        auto effectRenderLogic = GetNodeEffectRenderLogic( node );
        
        effectRenderLogic->RenderNode( renderer, node );
    }
}

// *********************************
//
NodeEffectRenderLogic *     RenderLogic::GetNodeEffectRenderLogic    ( SceneNode * node ) const
{
    assert( (unsigned int) node->GetNodeEffect()->GetType() < (unsigned int) NodeEffect::Type::T_TOTAL );

    return m_customNodeRenderLogic[ (unsigned int) node->GetNodeEffect()->GetType() ];
}

// *********************************
//
void    RenderLogic::DrawNode       ( Renderer * renderer, SceneNode * node )
{
	HPROFILER_SECTION( "RenderNode::renderer->Draw Anchor", PROFILER_THREAD1 );
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
        HPROFILER_SECTION( "RenderNode::RenderNode", PROFILER_THREAD1 );
        RenderNode  ( renderer, node->GetChild( i ) ); 
    }
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

// ***********************
//
void    RenderLogic::MakeScreenShot  ( const std::string& path, unsigned int numFrames )
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
