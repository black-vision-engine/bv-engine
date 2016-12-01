#include "stdafxBVApp.h"

#include "BVConfig.h"

#include <functional>

#include "Serialization/SerializationHelper.inl"
#include "VideoCardManager.h"

#define USE_READBACK_API
//#define FULLSCREEN_MODE
//#define DISPLAY_VIDEO_CARD_OUTPUT
#define PERSPECTIVE_CAMERA


namespace bv {


// *********************************
//
const std::string   BVConfig::CONFIG_PATH = "config.xml";


// *********************************
//FIXME: read default values from a configuration file
BVConfig::BVConfig                      ()
{
    m_deserializer.LoadFile( CONFIG_PATH );

    if( m_deserializer.EnterChild( "config" ) )
    {
        LoadProperties( m_deserializer );

        m_deserializer.ExitChild();  // config
    }

    m_defaultWidth = m_defaultWindowWidth = 1920;
    m_defaultHeight = m_defaultWindowHeight = 1080;

    m_pmFolder = m_properties[ "PMFolder" ];
    m_fullscreeMode = SerializationHelper::String2T< bool >( m_properties[ "FullScreen" ], true );
    m_isCameraPerspective = SerializationHelper::String2T< bool >( m_properties[ "PERSPECTIVE_CAMERA" ], true );
    m_readbackOn = SerializationHelper::String2T< bool >( m_properties[ "USE_READBACK_API" ], false );
    m_renderToSharedMemory = SerializationHelper::String2T< bool >( m_properties[ "Renderer/RenderToSharedMemory" ], false );
    m_sockerServerPort = SerializationHelper::String2T< Int32 >( m_properties[ "Network/SocketServer/Port" ], 12345 );

    m_useDebugLayer = SerializationHelper::String2T< bool >( m_properties[ "Debug/CommandsDebugLayer/UseDebugLayer" ], false );
    m_debugFilePath = m_properties[ "Debug/CommandsDebugLayer/FilePath" ];
    
    if( m_properties[ "Resolution" ] == "SD" )
    {
        m_defaultWidth = 720;
        m_defaultHeight = 576;
    }

    if( m_fullscreeMode )
    {
        m_windowMode = WindowMode::FULLSCREEN;
    }
    else
    {
        if( m_properties[ "Application/Window/Mode" ] == "MULTIPLE_SCREENS" )
        {
            m_windowMode = WindowMode::MULTIPLE_SCREENS;
        }
        else
        {
            m_windowMode = WindowMode::WINDOWED;
        }
        
        m_defaultWindowWidth = SerializationHelper::String2T< Int32 >( m_properties[ "Application/Window/Size/Width" ], m_defaultWidth );
        m_defaultWindowHeight = SerializationHelper::String2T< Int32 >( m_properties[ "Application/Window/Size/Height" ], m_defaultHeight );

        m_defaultWidth = SerializationHelper::String2T< Int32 >( m_properties[ "Application/Renderer/FrameBufferSize/Width" ], m_defaultWidth );
        m_defaultHeight = SerializationHelper::String2T< Int32 >( m_properties[ "Application/Renderer/FrameBufferSize/Height" ], m_defaultHeight );
    }

    m_vsync = SerializationHelper::String2T< bool >( m_properties[ "Application/VSYNC" ], true );
    if( m_vsync )
    {
        m_rendererInput.m_DisableVerticalSync = false;
        m_rendererInput.m_EnableGLFinish = true;
        m_rendererInput.m_EnableGLFlush = true;
        m_rendererInput.m_VerticalBufferFrameCount = 1;
    }
    else
    {
        m_rendererInput.m_DisableVerticalSync = true;
        m_rendererInput.m_EnableGLFinish = false;
        m_rendererInput.m_EnableGLFlush = false;
        m_rendererInput.m_VerticalBufferFrameCount = 0;
    }

    m_rendererInput.m_WindowHandle = nullptr;
    m_rendererInput.m_PixelFormat = 0;
    m_rendererInput.m_RendererDC = 0;

    m_fps = SerializationHelper::String2T< Int32 >( m_properties[ "Renderer/MaxFPS" ], 60 );
    m_frameTimeMillis = 1000 / m_fps;
    m_timerFPS = SerializationHelper::String2T< Int32 >( m_properties[ "Renderer/TimerFPS" ], 60 );

#ifdef DISPLAY_VIDEO_CARD_OUTPUT
    m_displayVideoCardOutput = true;
#else
    m_displayVideoCardOutput = false;
#endif

    m_eventLoopUpdateMillis = 20;

    m_defaultFOV = 90.f;
    m_defaultNearClippingPlane = 0.1f;
    m_defaultFarClippingPlane = 100.f;

    m_defaultFOV = SerializationHelper::String2T< Float32 >( m_properties[ "camera/fov" ], 90.f );

    m_defaultCameraPosition = glm::vec3( SerializationHelper::String2T< Float32 >( m_properties[ "camera/position/x" ], 0.f ),
                                                 SerializationHelper::String2T< Float32 >( m_properties[ "camera/position/y" ], 0.f ),
                                                 SerializationHelper::String2T< Float32 >( m_properties[ "camera/position/z" ], 0.f ) );
    m_defaultCameraDirection = glm::vec3( SerializationHelper::String2T< Float32 >( m_properties[ "camera/direction/x" ], 0.f ),
                                                 SerializationHelper::String2T< Float32 >( m_properties[ "camera/direction/y" ], 0.f ),
                                                 SerializationHelper::String2T< Float32 >( m_properties[ "camera/direction/z" ], 0.f ) );
    m_defaultCameraUp = glm::vec3( 0.f, 1.f, 0.f );

    m_defaultStatsMovingAverageWindowSize = 500; //500
    m_defaultWarmupRoundsStatsMAV = 10; //10
    m_defaultStatsRefreshMillisDelta = 1000;
    m_defaultStatsRecalcFramesDelta = m_defaultStatsMovingAverageWindowSize * m_defaultWarmupRoundsStatsMAV; //* 30
    m_defaultProfilerDisplayWaitMillis = 10000; //1000

    m_numRedbackBuffersPerRenderTarget = 4; //up to 200+, when 32 bit build is enabled

    m_defaultClearColor = glm::vec4( SerializationHelper::String2T< Float32 >( m_properties[ "Renderer/ClearColor/r" ], 0.f ),
                                            SerializationHelper::String2T< Float32 >( m_properties[ "Renderer/ClearColor/g" ], 0.f ),
                                            SerializationHelper::String2T< Float32 >( m_properties[ "Renderer/ClearColor/b" ], 0.f ),
                                            SerializationHelper::String2T< Float32 >( m_properties[ "Renderer/ClearColor/a" ], 0.f ) );
    m_defaultClearDepth = 1.0f;

    m_defaultSceneEnvVarName = "BV_DEFAULT_SCENE";
    m_sceneFromEnvName = m_properties[ "Debug/SceneFromEnvName" ];
    m_loadSceneFromEnv = SerializationHelper::String2T< bool >( m_properties[ "Debug/LoadSceneFromEnv" ], false );
    m_loadSceneFromProjectManager = m_properties[ "Debug/LoadSceneFromProjectManager" ];
    m_useVideoInputFeeding = SerializationHelper::String2T< bool >( m_properties[ "Debug/UseVideoInputFeeding" ], false );

    m_enableQueueLocking = SerializationHelper::String2T< bool >( m_properties[ "Application/EnableLockingQueue" ], false );
}

// *********************************
//
BVConfig::~BVConfig                     ()
{
}

// *********************************
//
void BVConfig::LoadProperties           ( const IDeserializer & deser, std::string path )
{
    auto name = deser.GetAttribute( "name" );
    auto value = deser.GetAttribute( "value" );

    if( path != "" )
    {
        path += "/";
    }
    path += name;

    if( value != "" )
    {
        m_properties[ path ] = value;
    }
    else
    {
        if( deser.EnterChild( "property" ) )
        {
            do
            {
                LoadProperties( deser, path );
            } while( deser.NextChild() );

            deser.ExitChild();  //property
        }
    }
}

// *********************************
//
const std::string &     BVConfig::PropertyValue         ( const std::string & key ) const
{
    return m_properties.at( key );
}

// *********************************
//
void                    BVConfig::SetPropertyValue      ( const std::string & key, const std::string & value )
{
    m_properties[ key ] = value;;
}

// *********************************
//
const IDeserializer &   BVConfig::GetNode               ( const std::string & nodeName ) const
{
    m_deserializer.Reset();
    m_deserializer.EnterChild( nodeName );
    return m_deserializer;
}

// *********************************
//
BVConfig & BVConfig::Instance     ()
{
    static BVConfig instance;

    return instance;
}

} //bv
