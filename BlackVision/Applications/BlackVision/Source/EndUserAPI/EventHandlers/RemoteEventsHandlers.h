#pragma once



namespace bv
{
class BVAppLogic;
class PluginEventsHandlers;
class SceneEventsHandlers;
class QueryHandlers;
class HightmapHandlers;
class EngineStateHandlers;

class RemoteEventsHandlers
{
public:
    PluginEventsHandlers*           m_pluginEvents;
    SceneEventsHandlers*            m_sceneEvents;
    QueryHandlers*                  m_queryEvents;
    HightmapHandlers*               m_heightmapEvents;
    EngineStateHandlers*            m_engineStateEvents;
public:
    RemoteEventsHandlers();
    ~RemoteEventsHandlers();

    void            InitializeHandlers      ( BVAppLogic* appLogic );

    // Still little hackisch
    void            UpdateHM                ();
};

} //bv
