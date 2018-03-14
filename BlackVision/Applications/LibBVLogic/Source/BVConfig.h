#pragma once

#include <map>

#include "Mathematics/glm_inc.h"

#include "Application/WindowedApplication.h" // enum WindowMode

#include "Serialization/XML/XMLDeserializer.h"
#include "Serialization/IDeserializer.h"
#include "Serialization/ISerializable.h"

#include "CoreDEF.h"

#include "Engine/Interfaces/IConfig.h"


namespace bv
{

class BVConfig : public IConfig
{
private:
    
    typedef std::map< std::string, std::string >    KVMap;

    static const std::string                CONFIG_PATH;

private:

    XMLDeserializer m_deserializer;

    KVMap           m_properties;

    Int32           m_defaultWindowWidth;
    Int32           m_defaultWindowHeight;

    Int32           m_defaultWidth;
    Int32           m_defaultHeight;

	//pablito
	WindowMode      m_windowMode;
	RendererInput	m_rendererInput;

    UInt32          m_eventLoopUpdateMillis;
    UInt32          m_fps;
    UInt32          m_frameTimeMillis;
    UInt32          m_timerFPS;

    bool            m_fullscreeMode;
    bool            m_readbackOn;
    bool            m_isCameraPerspective;
	bool			m_renderToSharedMemory;
    bool            m_vsync;

	Float32         m_globalGain;

    Float32         m_defaultFOV;
    Float32         m_defaultNearClippingPlane;
    Float32         m_defaultFarClippingPlane;

    glm::vec3       m_defaultCameraPosition;
    glm::vec3       m_defaultCameraUp;
    glm::vec3       m_defaultCameraDirection;

    UInt32          m_defaultStatsMovingAverageWindowSize;
    UInt32          m_defaultWarmupRoundsStatsMAV;
    UInt32          m_defaultStatsRefreshMillisDelta;
    UInt32          m_defaultStatsRecalcFramesDelta;
    UInt32          m_defaultProfilerDisplayWaitMillis;

    UInt32          m_numRedbackBuffersPerRenderTarget;

    glm::vec4       m_defaultClearColor;
    Float32         m_defaultClearDepth;

    std::string     m_sceneFromEnvName;
    std::string     m_defaultSceneEnvVarName;

    std::string     m_pmFolder;

    Int32           m_sockerServerPort;

    bool            m_useDebugLayer;
    std::string     m_debugFilePath;
    bool            m_loadSceneFromEnv;

    bool            m_enableQueueLocking;

    std::string     m_loadSceneFromProjectManager;
    std::string     m_onFailedTextureLoadBehavior;

private:

    BVConfig    ();

    void                        LoadProperties          ( const IDeserializer & deser, std::string path = "" );
    void                        SaveConfig              ( ISerializer & ser, std::string path = "" ) const;

    void                        InitDefaultConfiguration();
    void                        InitializeFromFile      ( const std::string & filePath );

public:

    explicit                    BVConfig                ( const std::string & configPath );
                                ~BVConfig               ();

    const std::string &         PropertyValue           ( const std::string & key ) const;
    void                        SetPropertyValue        ( const std::string & key, const std::string & value );

    const IDeserializer &       GetNode                 ( const std::string & node ) const;

    inline Int32                DefaultWindowWidth      () const;
    inline Int32                DefaultWindowHeight     () const;

    inline Int32                DefaultWidth            () const;
    inline Int32                DefaultHeight           () const;

    inline WindowMode           GetWindowMode	        () const;
	inline RendererInput        GetRendererInput	    () const;
	inline bool                 FullScreenMode          () const;
    inline bool                 ReadbackFlag            () const;
    inline bool                 IsCameraPerspactive     () const;
	inline bool                 RenderToSharedMemory    () const;

	inline Float32				GlobalGain				() const;

    inline UInt32               EventLoopUpdateMillis   () const;
    inline UInt32               FPS                     () const;
    inline UInt32               FrameTimeMillis         () const;
    inline UInt32               TimerFPS                () const;

    inline float                FOV                     () const;
    inline float                NearClippingPlane       () const;
    inline float                FarClippingPlane        () const;

    inline const glm::vec3 &    CameraPosition          () const;
    inline const glm::vec3 &    CameraUp                () const;
    inline const glm::vec3 &    CameraDirection         () const;

    inline const glm::vec4 &    ClearColor              () const;
    inline float                ClearDepth              () const;

    inline UInt32               StatsMAWindowSize       () const;
    inline UInt32               MAVWarmupRounds         () const;
    inline UInt32               StatsRefreshMillisDelta () const;
    inline UInt32               StatsRecalcFramesDelta  () const;

    inline UInt32               NumRedbackBuffersPerRT  () const;

    inline UInt32               ProfilerDispWaitMillis  () const;

    inline const char *         FrameStatsSection       () const;

    inline const std::string &  SceneFromEnvName            () const;
    inline const std::string &  LoadSceneFromProjectManager () const;
    inline std::string          DefaultSceneEnvVarName      () const;

    inline const std::string &  PMFolder                () const;

    inline Int32                SockerServerPort        () const;

    const IDeserializer &       GetNode                 ( int count, ... ) const;


    inline bool                 UseDebugLayer           () const;
    inline const std::string &  DebugFilePath           () const;
    inline bool                 LoadSceneFromEnv        () const;
    //inline bool                 UseVideoInputFeeding    () const;

    inline bool                 EnableLockingQueue      () const;
    inline const std::string &  OnFailedTexLoadBehavior () const;

    static BVConfig &           Instance  ();

};

#define DefaultConfig   BVConfig::Instance()

} //bv

#include "BVConfig.inl"
