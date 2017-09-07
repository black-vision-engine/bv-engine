#pragma once


namespace bv {

class BVAppLogic;
class PluginEventsHandlers;
class SceneEventsHandlers;
class QueryHandlers;
class HightmapHandlers;
class EngineStateHandlers;
class AssetHandlers;
class GlobalEffectHandler;
class TimelineHandlers;
class NodeLogicHandlers;
class VideoCardEventsHandlers;
class GenericEventsHandlers;
class GizmoHandlers;

class RemoteEventsHandlers
{
public:

    PluginEventsHandlers *      m_pluginEvents;
    SceneEventsHandlers *       m_sceneEvents;
    QueryHandlers *             m_queryEvents;
    HightmapHandlers *          m_heightmapEvents;
    EngineStateHandlers *       m_engineStateEvents;
    AssetHandlers *             m_assetEvents;
    GlobalEffectHandler *       m_globalEffectEvents;
    TimelineHandlers *          m_timelineHandlers;
    NodeLogicHandlers *         m_nodeLogicHandlers;
    VideoCardEventsHandlers *   m_videoCardHandlers;
    GenericEventsHandlers *     m_genericHandlers;
    GizmoHandlers *             m_gizmoHandlers;

public:

                    RemoteEventsHandlers    ();
                    ~RemoteEventsHandlers   ();

    void            InitializeHandlers      ( BVAppLogic * appLogic );

    // Still little hackisch
    void            UpdateHM                ();

};

} //bv
