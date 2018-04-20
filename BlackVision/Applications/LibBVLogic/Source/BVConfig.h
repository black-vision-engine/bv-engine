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



/**@page Config Config

[TOC]

<b>Full example:</b>


@code{.xml}
<config>

    <property name="PMFolder" value="../bv_media/" />

    <property name="UseReadbackAPI" value="true" />
    <property name="Resolution" value="HD" />

    <property name="Debug">
        <property name="LoadSceneFromEnv" value="false" />
        <property name="LoadSceneFromProjectManager" value="" />
        <property name="SceneFromEnvName" value="TWO_TEXTURED_RECTANGLES" />

        <property name="CommandsDebugLayer">
            <property name="UseDebugLayer" value="false" />
            <property name="FilePath" value="../../../../Logs/" />
        </property>
    </property>

    <property name="Plugins" >
        <property name="Textures" >
            <property name="OnFailedLoadBehavior" value="LoadChecker" />
        </property>
    </property>

    <property name="Audio" >
        <property name="GlobalGain" value="1.0" />
    </property>

    <property name="Renderer">
        <property name="MaxFPS" value="5000" />
        <property name="TimerFPS" value="50" />

        <property name="FrameBufferSize">
            <property name="Width" value="500" />
            <property name="Height" value="600" />
        </property>

        <property name="ClearColor" value="0,0,0,0" />

    </property>

    <property name="SharedMemory">
        <property name="Enable" value="true" />
        <property name="Name" value="BV" />
        <property name="Width" value="1920" />
        <property name="Height" value="1080" />
    </property>

    <property name="Application">
        <property name="VSync" value="false" />
        <property name="Window">
            <property name="FullScreen" value="false" />
            <property name="Mode" value="Windowed" />        <!-- rodzaj wyswietlanego okienka MultipleScreens|Windowed -->
            <property name="Size">
                <property name="Width" value="1920" />
                <property name="Height" value="1080" />
            </property>
        </property>

        <property name="Events" >
            <property name="MaxLoopUpdateTime" value="20" />
            <property name="EnableLockingQueue" value="false" />
        </property>
    </property>

    <property name="Network">
        <property name="SocketServer">
            <property name="Port" value="11011" />
        </property>
    </property>

    <property name="Camera">
        <property name="IsPerspective" value="true" />
        <property name="FOV" value="60" />
        <property name="Position" value="0,0,5" />
        <property name="Direction" value="0,0,-1" />
    </property>

    <videocards>
        <videocard name="BlueFish" deviceID="1" referenceMode="FreeRun" referenceH="0" referenceV="0" >
            <channels>
                <channel name="A" >
                    <output linkedVideoOutput="0" type="FILL_KEY" resolution="1080" refresh="5000" interlaced="true" flipped="true" />
                </channel>
                <channel name="B" >
                    <output linkedVideoOutput="1" type="FILL_KEY" resolution="1080" refresh="5000" interlaced="true" flipped="true" />
                </channel>
                <channel name="A" >
                    <input linkedVideoInput="0" type="FILL" resolution="1080" />
                </channel>
            </channels>
        </videocard>
    </videocards>

    <RenderChannels>

        <RenderChannel id="0" enabled="true" >
        </RenderChannel>

        <RenderChannel id="1" enabled="false" />
        <RenderChannel id="2" enabled="false" />
        <RenderChannel id="3" enabled="false" />
    </RenderChannels>

</config>
@endcode


@section Config_Application Application

Key                                     | Values        | Default Value        | Description
--------------------------------------- | ------------- | -------------------- | -----------
Application/VSync                       | bool          | true                 | Enable/Disable vertical synchronization with screen.
Application/Window/FullScreen           | bool          | false                | Set fullscreen mode.
Application/Window/Mode                 | enum          | WINDOWED             | MultipleScreens, WINDOWED. Ignored if FullScreen flag is st to true.
Application/Window/Size/Width           | enum          | Int32                | Width of application window.
Application/Window/Size/Height          | enum          | Int32                | Height of application window.


@subsection Config_Events Events

Key                                     | Values        | Default Value        | Description
--------------------------------------- | ------------- | -------------------- | -----------
Application/Events/MaxLoopUpdateTime    | UInt32        | 20                   | Maximal time spent on processing events during single frame.
Application/Events/EnableLockingQueue   | bool          | false                | Enables event for blocking processing events.

<b>Examples:</b>

@code{.xml}
<config>
    <property name="Application">

        <property name="Events" >
            <property name="MaxLoopUpdateTime" value="20" />
            <property name="EnableLockingQueue" value="false" />
        </property>
    </property>
</config>
@endcode

@section Config_Renderer Renderer

Key                                     | Values        | Default Value        | Description
--------------------------------------- | ------------- | -------------------- | -----------
MaxFPS                                  | UInt32        | 60000                | -
TimerFPS                                | UInt32        | 60                   | -
ClearColor                              | vec4          | 0.0, 0.0, 0.0, 0.0   | Default color to render target clear operation.
FrameBufferSize/Width                   | UInt32        | 1920                 | -
FrameBufferSize/Height                  | UInt32        | 1080                 | -

<b>Examples:</b>

@code{.xml}
<config>
    <property name="Renderer">
        <property name="MaxFPS" value="5000" />
        <property name="TimerFPS" value="50" />

        <property name="FrameBufferSize">
            <property name="Width" value="500" />
            <property name="Height" value="600" />
        </property>

        <property name="ClearColor" value="0,0,0,0" />

    </property>
</config>
@endcode


@section Config_SharedMemory SharedMemory

Key                                     | Values        | Default Value        | Description
--------------------------------------- | ------------- | -------------------- | -----------
Enable                                  | bool          | false                | Enables rendering to shared memory.
Width/Height                            | UInt32        | 1920x1080            | Size od shared memory buffer.
Name                                    | string        | "BV"                 | Name of shared memory buffer to access in external application.

<b>Examples:</b>

@code{.xml}
<config>
    <property name="SharedMemory">
        <property name="Enable" value="false" />
        <property name="Name" value="BV" />
        <property name="Width" value="1920" />
        <property name="Height" value="1080" />
    </property>
</config>
@endcode

@section Config_Network Network

Key                                     | Values        | Default Value        | Description
--------------------------------------- | ------------- | -------------------- | -----------
SocketServer/Port                       | UInt32        | 12345                | Connection port for external tools (events API)

<b>Examples:</b>

@code{.xml}
<config>
    <property name="Network">
        <property name="SocketServer">
            <property name="Port" value="11011" />
        </property>
    </property>
</config>
@endcode

@section Config_Plugins Plugins

@subsection Config_Textures Textures

Key                                     | Values        | Default Value        | Description
--------------------------------------- | ------------- | -------------------- | -----------
Plugins/Textures/OnFailedLoadBehavior   | string        | LoadChecker          | Texture plugins behavior, when loaded texture doesn't exist. Posible values: **LoadChecker**, **LoadTransparent**, **LeavePrevious**.

<b>Example:</b>

@code{.xml}
<config>
    <property name="Plugins" >
        <property name="Textures" >
            <property name="OnFailedLoadBehavior" value="LoadChecker" />
        </property>
    </property>
</config>
@endcode

@section Config_Audio Audio

Key                                     | Values        | Default Value        | Description
--------------------------------------- | ------------- | -------------------- | -----------
GlobalGain                              | float         | 1.0                  | Audio global gain.

@section Config_Debug Debug

Key                                     | Values        | Default Value        | Description
--------------------------------------- | ------------- | -------------------- | -----------
LoadSceneFromEnv                        | bool          | false                | BV will load scene using environment variable.
SceneFromEnvName                        | string        | ""                   | One of embeded test scenes.
LoadSceneFromProjectManager             | string        | ""                   | Scene name from ProjectManager to load.
CommandsDebugLayer/UseDebugLayer        | bool          | false                | Record all events coming to BV through TCP connection.
CommandsDebugLayer/FilePath             | string        | ""                   | File to save recorded events.

<b>Examples:</b>

@code{.xml}
<config>
    <property name="Debug">
        <property name="LoadSceneFromEnv" value="false" />
        <property name="LoadSceneFromProjectManager" value="" />
        <property name="SceneFromEnvName" value="TWO_TEXTURED_RECTANGLES" />

        <property name="CommandsDebugLayer">
            <property name="UseDebugLayer" value="false" />
            <property name="FilePath" value="../../../../Logs/" />
        </property>
    </property>
</config>
@endcode

@section Config_Camera Camera

Default camera parameters. <b>Note that these values aren't used in implementation</b> and remain for future use.

@code{.xml}
<config>
    <property name="Camera">
        <property name="IsPerspective" value="true" />
        <property name="FOV" value="60" />
        <property name="Position" value="0,0,5" />
        <property name="Direction" value="0,0,-1" />
    </property>
</config>
@endcode

@section Config_RenderChannels Render Channels


@code{.xml}
<config>
    <RenderChannels>

        <RenderChannel id="0" enabled="true" >
            <VideoOutput id="0" />
        </RenderChannel>

        <RenderChannel id="1" enabled="true" >
            <VideoOutput id="1" />
        </RenderChannel>

        <RenderChannel id="2" enabled="false" />
        <RenderChannel id="3" enabled="false" />
    </RenderChannels>
</config>
@endcode



@section Config_VideoCards Video cards


@code{.xml}
<config>

    <videocards>
        <videocard name="BlueFish" deviceID="1" referenceMode="FreeRun" referenceH="0" referenceV="0" >
            <channels>
                <channel name="A" >
                    <output linkedVideoOutput="0" type="FILL_KEY" resolution="1080" refresh="5000" interlaced="true" flipped="true" />
                </channel>
                <channel name="B" >
                    <output linkedVideoOutput="1" type="FILL_KEY" resolution="1080" refresh="5000" interlaced="true" flipped="true" />
                </channel>
                <channel name="A" >
                    <input linkedVideoInput="0" type="FILL" resolution="1080" />
                </channel>
            </channels>
        </videocard>
    </videocards>

</config>
@endcode
*/



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
    bool            m_vsync;
    bool			m_renderToSharedMemory;

    std::string     m_shmName;
    UInt32          m_shmWidth;
    UInt32          m_shmHeight;

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

    void                        InitDefaultConfiguration();
    void                        InitializeFromFile      ( const std::string & filePath );
    void                        RecomputeDependentValues();

public:

    explicit                    BVConfig                ( const std::string & configPath );
                                ~BVConfig               ();

    void                        SaveConfig              ( std::string path = "" ) const;
    void                        SaveProperties          ( ISerializer & ser ) const;
    void                        SaveRenderChannels      ( ISerializer & ser ) const;
    void                        SaveVideoCards          ( ISerializer & ser ) const;

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
    inline UInt32               SharedMemoryWidth       () const;
    inline UInt32               SharedMemoryHeight      () const;
    inline const std::string &  SharedMemoryBufferName  () const;

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

private:

    enum class EntryType
    {
        Optional,
        Required
    };

    template< typename PropertyType >
    using ConfigPropertyPtr = PropertyType ( BVConfig::* );

    template< typename PropertyType >
    void            LoadPropertyValueOrSetDefault                   ( const char * propertyPath, ConfigPropertyPtr< PropertyType > member, EntryType type );

    template<>
    void            LoadPropertyValueOrSetDefault< std::string >    ( const char * propertyPath, ConfigPropertyPtr< std::string > member, EntryType type );
};

#define DefaultConfig   BVConfig::Instance()

} //bv

#include "BVConfig.inl"
