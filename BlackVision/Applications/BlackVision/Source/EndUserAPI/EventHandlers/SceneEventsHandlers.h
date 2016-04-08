#pragma once

#include "Engine/Events/Events.h"
#include <map>
#include <string>

namespace bv
{
class BVAppLogic;

namespace model
{
    class SceneModel;
    DEFINE_PTR_TYPE( SceneModel )
}


typedef std::map< bv::model::SceneModelPtr, bool >   SceneVisibilityStateMap;

class SceneEventsHandlers
{
private:
    BVAppLogic*                 m_appLogic;

    // Save scene or preset state
    SceneVisibilityStateMap     m_scenesVisibilityState;        // Used to save visibility state while making scene or preset thumbnail.
    bool                        m_closeSavedPreset;
    model::SceneModelPtr        m_savedScene;

public:
    SceneEventsHandlers( BVAppLogic* appLogic );
    ~SceneEventsHandlers();

    void        NodeStructure       ( bv::IEventPtr evt );
    void        PluginStructure     ( bv::IEventPtr evt );
    void        ProjectStructure    ( bv::IEventPtr evt );
    void        SceneStructure	    ( bv::IEventPtr evt );

    void        LightsManagement	( bv::IEventPtr evt );

    void        SceneVariable       ( bv::IEventPtr evt );

private:
    void        RequestThumbnail    ( bv::model::SceneModelPtr scene, const std::string & saveTo );
    void        ThumbnailRendered   ( bv::IEventPtr evt );

    void        SaveVisibilityState     ( const std::string & sceneName );
    void        RestoreVisibilityState  ();
};


} //bv
