#include "stdafxBVApp.h"

#include "BVConfig.h"

#include <functional>

#include "Serialization/XML/XMLSerializer.h"
#include "Serialization/SerializationHelper.inl"

#include "VideoCardManager.h"

#include "UseLoggerBVAppModule.h"

#define USE_READBACK_API
//#define FULLSCREEN_MODE
#define PERSPECTIVE_CAMERA


namespace bv {


// *********************************
//
const std::string   BVConfig::CONFIG_PATH = "config.xml";
const std::string   EMPTY_STRING = "";

// *********************************
//
void    BVConfig::InitDefaultConfiguration()
{
    m_defaultWidth = m_defaultWindowWidth = 1920;
    m_defaultHeight = m_defaultWindowHeight = 1080;

    m_pmFolder = "bv_media";
    m_fullscreeMode = false;
    m_isCameraPerspective = true;
    m_readbackOn = false;
    m_renderToSharedMemory = false;

    m_globalGain = 1.f;

    m_sockerServerPort = 12345;

    m_useDebugLayer = false;
    m_debugFilePath = "";

    m_windowMode = WindowMode::WINDOWED;

    m_vsync = false;
    m_rendererInput.m_DisableVerticalSync = true;
    m_rendererInput.m_EnableGLFinish = false;
    m_rendererInput.m_EnableGLFlush = false;
    m_rendererInput.m_VerticalBufferFrameCount = 0;

    m_rendererInput.m_WindowHandle = nullptr;
    m_rendererInput.m_PixelFormat = 0;
    m_rendererInput.m_RendererDC = 0;

    m_fps = 60000;
    m_frameTimeMillis = 1000 / m_fps;
    m_timerFPS = 60;

    m_eventLoopUpdateMillis = 20;

    m_defaultFOV = 90.f;
    m_defaultNearClippingPlane = 0.1f;
    m_defaultFarClippingPlane = 100.f;

    m_defaultFOV = 90.f;

    m_defaultCameraPosition = glm::vec3( 0.f, 0.f, 5.f );
    m_defaultCameraDirection = glm::vec3( 0.f , 0.f, 5.f );
    m_defaultCameraUp = glm::vec3( 0.f, 1.f, 0.f );

    m_defaultStatsMovingAverageWindowSize = 500; //500
    m_defaultWarmupRoundsStatsMAV = 10; //10
    m_defaultStatsRefreshMillisDelta = 1000;
    m_defaultStatsRecalcFramesDelta = m_defaultStatsMovingAverageWindowSize * m_defaultWarmupRoundsStatsMAV; //* 30
    m_defaultProfilerDisplayWaitMillis = 10000; //1000

    m_numRedbackBuffersPerRenderTarget = 4; //up to 200+, when 32 bit build is enabled

    m_defaultClearColor = glm::vec4( 0.f, 0.f, 0.f, 0.f );
    m_defaultClearDepth = 1.0f;

    m_defaultSceneEnvVarName = "";
    m_sceneFromEnvName = "";
    m_loadSceneFromEnv = false;
    m_loadSceneFromProjectManager = "";

    m_enableQueueLocking = false;

    m_onFailedTextureLoadBehavior = "LoadChecker";
}

// *********************************
//FIXME: read default values from a configuration file
BVConfig::BVConfig                      ()
{
    InitializeFromFile( CONFIG_PATH );
}

// ***********************
// This version is used mostly in tests.
BVConfig::BVConfig                      ( const std::string & configPath )
{
    InitializeFromFile( configPath );
}

// ***********************
//
void                    BVConfig::InitializeFromFile        ( const std::string & filePath )
{
    InitDefaultConfiguration();

    if( Path::Exists( filePath ) )
    {
        m_deserializer.LoadFile( filePath );

        if( m_deserializer.EnterChild( "config" ) )
        {
            LoadProperties( m_deserializer );

            m_deserializer.ExitChild();  // config
        }

        m_defaultWidth = m_defaultWindowWidth = 1920;
        m_defaultHeight = m_defaultWindowHeight = 1080;

        m_pmFolder = m_properties[ "PMFolder" ];
        m_fullscreeMode = Convert::String2T< bool >( m_properties[ "FullScreen" ], true );
        m_isCameraPerspective = Convert::String2T< bool >( m_properties[ "PERSPECTIVE_CAMERA" ], true );
        m_readbackOn = Convert::String2T< bool >( m_properties[ "USE_READBACK_API" ], false );

        m_renderToSharedMemory = Convert::String2T< bool >( m_properties[ "SharedMemory/Enable" ], false );

        m_globalGain = Convert::String2T< Float32 >( m_properties[ "Audio/GlobalGain" ], 1.f );

        m_sockerServerPort = Convert::String2T< Int32 >( m_properties[ "Network/SocketServer/Port" ], 12345 );

        m_useDebugLayer = Convert::String2T< bool >( m_properties[ "Debug/CommandsDebugLayer/UseDebugLayer" ], false );
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

            m_defaultWindowWidth = Convert::String2T< Int32 >( m_properties[ "Application/Window/Size/Width" ], m_defaultWidth );
            m_defaultWindowHeight = Convert::String2T< Int32 >( m_properties[ "Application/Window/Size/Height" ], m_defaultHeight );

            m_defaultWidth = Convert::String2T< Int32 >( m_properties[ "Application/Renderer/FrameBufferSize/Width" ], m_defaultWidth );
            m_defaultHeight = Convert::String2T< Int32 >( m_properties[ "Application/Renderer/FrameBufferSize/Height" ], m_defaultHeight );
        }

        m_vsync = Convert::String2T< bool >( m_properties[ "Application/VSYNC" ], true );
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

        m_fps = Convert::String2T< Int32 >( m_properties[ "Renderer/MaxFPS" ], 60 );
        m_frameTimeMillis = 1000 / m_fps;
        m_timerFPS = Convert::String2T< Int32 >( m_properties[ "Renderer/TimerFPS" ], 60 );

        m_eventLoopUpdateMillis = Convert::String2T< UInt32 >( m_properties[ "Application/Events/MaxLoopUpdateTime" ], m_eventLoopUpdateMillis );

        m_defaultFOV = 90.f;
        m_defaultNearClippingPlane = 0.1f;
        m_defaultFarClippingPlane = 100.f;

        m_defaultFOV = Convert::String2T< Float32 >( m_properties[ "camera/fov" ], 90.f );

        m_defaultCameraPosition = glm::vec3( Convert::String2T< Float32 >( m_properties[ "camera/position/x" ], 0.f ),
            Convert::String2T< Float32 >( m_properties[ "camera/position/y" ], 0.f ),
            Convert::String2T< Float32 >( m_properties[ "camera/position/z" ], 0.f ) );
        m_defaultCameraDirection = glm::vec3( Convert::String2T< Float32 >( m_properties[ "camera/direction/x" ], 0.f ),
            Convert::String2T< Float32 >( m_properties[ "camera/direction/y" ], 0.f ),
            Convert::String2T< Float32 >( m_properties[ "camera/direction/z" ], 0.f ) );
        m_defaultCameraUp = glm::vec3( 0.f, 1.f, 0.f );

        m_defaultStatsMovingAverageWindowSize = 500; //500
        m_defaultWarmupRoundsStatsMAV = 10; //10
        m_defaultStatsRefreshMillisDelta = 1000;
        m_defaultStatsRecalcFramesDelta = m_defaultStatsMovingAverageWindowSize * m_defaultWarmupRoundsStatsMAV; //* 30
        m_defaultProfilerDisplayWaitMillis = 10000; //1000

        m_numRedbackBuffersPerRenderTarget = 4; //up to 200+, when 32 bit build is enabled

        m_defaultClearColor = glm::vec4( Convert::String2T< Float32 >( m_properties[ "Renderer/ClearColor/r" ], 0.f ),
            Convert::String2T< Float32 >( m_properties[ "Renderer/ClearColor/g" ], 0.f ),
            Convert::String2T< Float32 >( m_properties[ "Renderer/ClearColor/b" ], 0.f ),
            Convert::String2T< Float32 >( m_properties[ "Renderer/ClearColor/a" ], 0.f ) );
        m_defaultClearDepth = 1.0f;

        m_defaultSceneEnvVarName = "BV_DEFAULT_SCENE";
        m_sceneFromEnvName = m_properties[ "Debug/SceneFromEnvName" ];
        m_loadSceneFromEnv = Convert::String2T< bool >( m_properties[ "Debug/LoadSceneFromEnv" ], false );
        m_loadSceneFromProjectManager = m_properties[ "Debug/LoadSceneFromProjectManager" ];

        m_enableQueueLocking = Convert::String2T< bool >( m_properties[ "Application/Events/EnableLockingQueue" ], false );

        m_onFailedTextureLoadBehavior = m_properties[ "Plugins/Textures/OnFailedLoadBehavior" ];
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Default config file 'config.xml' reading error. Loading default configuration.";
    }
}


// *********************************
//
const IDeserializer &  BVConfig::GetNode                    ( int count, ... ) const
{
    va_list args;
    va_start( args, count );

    std::string path;

    if( count > 0 )
    {
        m_deserializer.Reset();

        for( int i = 0; i < count; ++i )
        {
            auto  n = va_arg( args, const char * );
            path += n + '/';

            if( !m_deserializer.EnterChild( n ) )
            {
                LOG_MESSAGE( SeverityLevel::error ) << "Config path '" << path << "' doesn't exist.";
                break;
            }
        }

        va_end( args );
    }

    return m_deserializer;
}

// *********************************
//
//SizeType             BVConfig::GetNumRenderChannels    () const
//{
//    m_deserializer.Reset();
//
//    SizeType i = 0;
//
//    if( m_deserializer.EnterChild( "config" ) &&
//        m_deserializer.EnterChild( "videocards" ) &&
//        m_deserializer.EnterChild( "RenderChannels" )
//       )
//    {
//        m_deserializer.EnterChild( "RenderChannel" );
//
//        do
//        {
//            if( m_deserializer.GetName() == "RenderChannel" )
//                i++;
//        }
//        while( m_deserializer.NextChild() );
//    }
//
//    return i;
//}
//
//// *********************************
////
//SizeType             BVConfig::GetNumOutputs           ( const std::string & rcID ) const
//{
//    m_deserializer.Reset();
//
//    SizeType numOfOutputs = 0;
//    SizeType i = 0;
//
//    if( m_deserializer.EnterChild( "config" ) && m_deserializer.EnterChild( "RenderChannels" ) )
//    {
//        do
//        {
//            if( !m_deserializer.EnterChild( "RenderChannel" ) || !m_deserializer.ExitChild() )
//            {
//                return 0;
//            }
//        }
//        while( m_deserializer.NextChild() && i < rcIdx );
//
//        if( m_deserializer.EnterChild( "RenderChannel" ) )
//        {
//            do
//            {
//                if( m_deserializer.EnterChild( "Output" ) && m_deserializer.ExitChild() )
//                {
//                    numOfOutputs++;
//                }
//            }
//            while( m_deserializer.NextChild() );
//
//            m_deserializer.ExitChild(); // RenderChannel
//        }
//
//        m_deserializer.ExitChild(); // RenderChannels
//        m_deserializer.ExitChild(); // config
//    }
//
//    return numOfOutputs;
//}
//
//// *********************************
////
//std::vector< BVConfig::KVMap >      BVConfig::GetOutputsProp          ( const std::string & rcID ) const
//{
//    m_deserializer.Reset();
//
//    std::vector< BVConfig::KVMap > properties;
//
//    if( m_deserializer.EnterChild( "config" ) &&
//        m_deserializer.EnterChild( "videocards" ) && 
//        m_deserializer.EnterChild( "RenderChannels" ) )
//    {
//        m_deserializer.EnterChild( "RenderChannel" );
//        do
//        {
//            if( m_deserializer.GetName() == "RenderChannel" )
//            {
//                if( m_deserializer.GetAttribute( "id" ) == rcID )
//                {
//                    if( m_deserializer.EnterChild( "Output" ) )
//                    {
//                        do
//                        {
//                            BVConfig::KVMap p;
//                            p[ "id" ] = m_deserializer.GetAttribute( "id" );
//                            p[ "width" ] = m_deserializer.GetAttribute( "width" );
//                            p[ "height" ] = m_deserializer.GetAttribute( "height" );
//
//                            properties.push_back( p );
//                        }
//                        while( m_deserializer.NextChild() );
//
//                        m_deserializer.ExitChild();
//                    }
//                }
//            }
//
//        }
//        while( m_deserializer.NextChild() );
//        
//        m_deserializer.ExitChild(); // RenderChannel
//    }
//
//    return properties;
//}

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

// ***********************
//
void                    BVConfig::SaveConfig            ( ISerializer & /*ser*/, std::string /*path*/ ) const
{
    if( Path::Exists( CONFIG_PATH ) )
    {
        //XMLSerializer ser( nullptr );

        //if( m_deserializer.EnterChild( "config" ) )
        //{
        //    for( auto iter = m_properties.begin(); iter != m_properties.end(); ++iter )
        //    {


        //    }

        //    m_deserializer.ExitChild();  // config
        //}

        //ser.Save( CONFIG_PATH );
    }
}

// *********************************
//
const std::string &     BVConfig::PropertyValue         ( const std::string & key ) const
{
    auto iter = m_properties.find( key );
    if( iter != m_properties.end() )
        return iter->second;
    return EMPTY_STRING;
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
