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


namespace bv
{



// ***********************
//
template< typename PropertyType >
void            BVConfig::LoadPropertyValueOrSetDefault       ( const char * propertyPath, BVConfig::ConfigPropertyPtr< PropertyType > member, EntryType type )
{
    auto expPropValue = Convert::String2T< PropertyType >( m_properties[ propertyPath ] );
    if( expPropValue.IsValid() )
    {
        this->*member = expPropValue.GetVal();
    }
    else
    {
        if( type == EntryType::Required )
        {
            LOG_MESSAGE( SeverityLevel::warning ) << "Invalid config entry. Property [" << propertyPath << "], value [" << m_properties[ propertyPath ] << "]. Default value set.";
        }

        // If property is invalid we treat current value of field in member pointer as default.
        // Entry in m_properties mus be equal to default value.
        m_properties[ propertyPath ] = Convert::T2String( this->*member );
    }
}



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

        LoadPropertyValueOrSetDefault( "PMFolder", &BVConfig::m_pmFolder, EntryType::Required );
        LoadPropertyValueOrSetDefault( "FullScreen", &BVConfig::m_fullscreeMode, EntryType::Required );
        LoadPropertyValueOrSetDefault( "USE_READBACK_API", &BVConfig::m_readbackOn, EntryType::Required );

        LoadPropertyValueOrSetDefault( "SharedMemory/Enable", &BVConfig::m_renderToSharedMemory, EntryType::Required );

        LoadPropertyValueOrSetDefault( "Audio/GlobalGain", &BVConfig::m_globalGain, EntryType::Required );

        LoadPropertyValueOrSetDefault( "Network/SocketServer/Port", &BVConfig::m_sockerServerPort, EntryType::Required );

        LoadPropertyValueOrSetDefault( "Debug/CommandsDebugLayer/UseDebugLayer", &BVConfig::m_useDebugLayer, EntryType::Optional );
        LoadPropertyValueOrSetDefault( "Debug/CommandsDebugLayer/FilePath", &BVConfig::m_debugFilePath, EntryType::Optional );


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
            else if( m_properties[ "Application/Window/Mode" ] == "WINDOWED" )
            {
                m_windowMode = WindowMode::WINDOWED;
            }
            else
            {
                LOG_MESSAGE( SeverityLevel::warning ) << "Invalid [Application/Window/Mode], value [" << m_properties[ "Application/Window/Mode" ] << "].";
            }

            LoadPropertyValueOrSetDefault( "Application/Window/Size/Width", &BVConfig::m_defaultWindowWidth, EntryType::Required );
            LoadPropertyValueOrSetDefault( "Application/Window/Size/Height", &BVConfig::m_defaultWindowHeight, EntryType::Required );

            LoadPropertyValueOrSetDefault( "Renderer/FrameBufferSize/Width", &BVConfig::m_defaultWidth, EntryType::Required );
            LoadPropertyValueOrSetDefault( "Renderer/FrameBufferSize/Height", &BVConfig::m_defaultHeight, EntryType::Required );
        }

        LoadPropertyValueOrSetDefault( "Application/VSYNC", &BVConfig::m_vsync, EntryType::Required );

        LoadPropertyValueOrSetDefault( "Renderer/MaxFPS", &BVConfig::m_fps, EntryType::Required );
        LoadPropertyValueOrSetDefault( "Renderer/TimerFPS", &BVConfig::m_timerFPS, EntryType::Required );

        LoadPropertyValueOrSetDefault( "Application/Events/MaxLoopUpdateTime", &BVConfig::m_eventLoopUpdateMillis, EntryType::Required );
        LoadPropertyValueOrSetDefault( "Application/Events/EnableLockingQueue", &BVConfig::m_enableQueueLocking, EntryType::Optional );

        LoadPropertyValueOrSetDefault( "Camera/IsPerspective", &BVConfig::m_isCameraPerspective, EntryType::Required );
        LoadPropertyValueOrSetDefault( "Camera/FOV", &BVConfig::m_defaultFOV, EntryType::Required );
        LoadPropertyValueOrSetDefault( "Camera/Position", &BVConfig::m_defaultCameraPosition, EntryType::Required );
        LoadPropertyValueOrSetDefault( "Camera/Direction", &BVConfig::m_defaultCameraDirection, EntryType::Required );

        LoadPropertyValueOrSetDefault( "Renderer/ClearColor", &BVConfig::m_defaultClearColor, EntryType::Required );

        LoadPropertyValueOrSetDefault( "BV_DEFAULT_SCENE", &BVConfig::m_defaultSceneEnvVarName, EntryType::Required );
        LoadPropertyValueOrSetDefault( "Debug/SceneFromEnvName", &BVConfig::m_sceneFromEnvName, EntryType::Required );
        LoadPropertyValueOrSetDefault( "Debug/LoadSceneFromEnv", &BVConfig::m_loadSceneFromEnv, EntryType::Required );
        LoadPropertyValueOrSetDefault( "Debug/LoadSceneFromProjectManager", &BVConfig::m_loadSceneFromProjectManager, EntryType::Required );

        LoadPropertyValueOrSetDefault( "Plugins/Textures/OnFailedLoadBehavior", &BVConfig::m_onFailedTextureLoadBehavior, EntryType::Required );

        RecomputeDependentValues();
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Config file [" << filePath << "] doesn't exist. Loading default configuration.";
    }
}

// ***********************
//
void                    BVConfig::RecomputeDependentValues  ()
{
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

    m_frameTimeMillis = 1000 / m_fps;
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
void                    BVConfig::LoadProperties        ( const IDeserializer & deser, std::string path )
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
UInt32                  FindCommonPathPart              ( StringVector & path1, StringVector & path2 )
{
    UInt32 eqNesting = 0;

    while( path1.size() > eqNesting
        && path2.size() > eqNesting
        && path1[ eqNesting ] == path2[ eqNesting ] )
    {
        eqNesting++;
    }

    return eqNesting;
}

// ***********************
//
void                    BVConfig::SaveConfig            ( std::string path ) const
{
    if( /*Path::Exists( path )*/ true )
    {
        XMLSerializer ser( nullptr );

        ser.EnterChild( "config" );

        UInt32 prevNesting = 0;
        StringVector prevPath;

        // We use fact, that properties are sorted in map.
        for( auto iter = m_properties.begin(); iter != m_properties.end(); ++iter )
        {
            Path propPath = iter->first;
            StringVector curPath = propPath.Split();

            // Find common part of path.
            UInt32 eqNesting = FindCommonPathPart( prevPath, curPath );

            // In previous interation we were more nested in serializer.
            if( prevNesting > eqNesting )
            {
                UInt32 numExits = prevNesting - eqNesting;
                while( numExits > 0 )
                {
                    ser.ExitChild();
                    numExits--;
                }
            }

            // We must EnterChild beginning from eqNesting index in curPath vector.
            UInt32 nesting = eqNesting;
            UInt32 maxNesting = (UInt32)curPath.size() - 1;
            while( nesting <= maxNesting )
            {
                ser.EnterChild( "property" );
                ser.SetAttribute( "name", curPath[ nesting ] );

                nesting++;
            }

            prevNesting = nesting;

            // We are in leaf of properties tree. Set Value.
            ser.SetAttribute( "value", iter->second );

            prevPath = curPath;
        }

        ser.ExitChild();  // config

        ser.Save( path );
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
