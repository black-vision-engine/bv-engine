#pragma once



namespace bv
{
class BVAppLogic;
class PluginEventsHandlers;
class SceneEventsHandlers;
class QueryHandlers;

class RemoteEventsHandlers
{
public:
    PluginEventsHandlers*           m_pluginEvents;
    SceneEventsHandlers*            m_sceneEvents;
    QueryHandlers*                  m_queryEvents;
public:
    RemoteEventsHandlers();
    ~RemoteEventsHandlers();

    void            InitializeHandlers      ( BVAppLogic* appLogic );
};

} //bv
