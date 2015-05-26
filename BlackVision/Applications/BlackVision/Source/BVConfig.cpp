#include "BVConfig.h"

//#define USE_READBACK_API
//#define FULLSCREEN_MODE
//#define PERSPECTIVE_CAMERA

namespace bv
{

// *********************************
//FIXME: read default values from a configuration file
BVConfig::BVConfig                      ()
{
    ConfigManager::LoadXMLConfig();

    m_defaultWidth      = 1920;
    m_defaultHeight     = 1080;

    if(ConfigManager::GetBool("FullScreen"))
    {
	    m_windowMode            = WindowMode::FULLSCREEN;
        m_defaultWindowWidth    = m_defaultWidth;
    	m_defaultWindowHeight   = m_defaultHeight;
    }else{
        m_fullscreeMode         = false;
        
        if(ConfigManager::GetString("Application/Window/Mode")=="MULTIPLE_SCREENS")
        {
            m_windowMode            = WindowMode::MULTIPLE_SCREENS;
        }else{
            m_windowMode            = WindowMode::WINDOWED;
        }
        m_defaultWindowWidth    = ConfigManager::GetInt("Application/Window/Size/Width");
        m_defaultWindowHeight   = ConfigManager::GetInt("Application/Window/Size/Height");

		m_defaultWidth    = ConfigManager::GetInt("Application/Renderer/FrameBufferSize/Width");
        m_defaultHeight   = ConfigManager::GetInt("Application/Renderer/FrameBufferSize/Height");
    }

	

	 if(ConfigManager::GetBool("Application/VSYNC")==true)
	 {
		m_RendererInput.m_DisableVerticalSync	= false;
		m_RendererInput.m_EnableGLFinish		= true;
		m_RendererInput.m_EnableGLFlush			= true;
		m_RendererInput.m_VerticalBufferFrameCount = 1;
	 }else{
		m_RendererInput.m_DisableVerticalSync	= true;
		m_RendererInput.m_EnableGLFinish		= false;
		m_RendererInput.m_EnableGLFlush			= false;
		m_RendererInput.m_VerticalBufferFrameCount = 0;
	 }

	m_RendererInput.m_WindowHandle			= nullptr;
    m_RendererInput.m_PixelFormat			= 0;
    m_RendererInput.m_RendererDC			= 0;

	

    
    if(ConfigManager::GetString("Resolution")=="SD")
    {
        m_defaultWidth      = 720;
        m_defaultHeight     = 576;
    }


    m_fps               = ConfigManager::GetInt("Renderer/MaxFPS");
    m_frameTimeMillis   = 1000 / m_fps;

if(ConfigManager::GetBool("USE_READBACK_API"))
    m_readbackOn        = true;
else
    m_readbackOn        = false;


if(ConfigManager::GetBool("PERSPECTIVE_CAMERA"))
    m_isCameraPerspective = true;
else
    m_isCameraPerspective = false;


    m_eventLoopUpdateMillis = 20;

    m_defaultFOV                = 90.f;
    m_defaultNearClippingPlane  = 0.1f;
    m_defaultFarClippingPlane   = 100.f;

    m_defaultCameraPosition  = glm::vec3( ConfigManager::GetFloat("camera/position/x"),ConfigManager::GetFloat("camera/position/y"),ConfigManager::GetFloat("camera/position/z"));
    m_defaultCameraDirection = glm::vec3( 0.f, 0.f, 0.f );
    m_defaultCameraUp        = glm::vec3( 0.f, 1.f, 0.f );

    m_defaultStatsMovingAverageWindowSize   = 500; //500
    m_defaultWarmupRoundsStatsMAV           = 10; //10
    m_defaultStatsRefreshMillisDelta        = 1000;
    m_defaultStatsRecalcFramesDelta         = m_defaultStatsMovingAverageWindowSize * m_defaultWarmupRoundsStatsMAV; //* 30
    m_defaultProfilerDisplayWaitMillis      = 10000; //1000

    m_numRedbackBuffersPerRenderTarget      = 4; //up to 200+, when 32 bit build is enabled

    m_defaultClearColor     = glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f );
    m_defaultClearDepth     = 1.0f;

    m_defaultSceneEnvVarName    = "BV_DEFAULT_SCENE";
}

// *********************************
//
BVConfig::~BVConfig                     ()
{
}

// *********************************
//
const BVConfig & BVConfig::Instance     ()
{
    static BVConfig instance;

    return instance;
}

} //bv
