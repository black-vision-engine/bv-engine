#pragma once

#include "Engine/Events/Events.h"

namespace bv
{
class BVAppLogic;

class SceneEventsHandlers
{
private:
    BVAppLogic*         m_appLogic;
public:
    SceneEventsHandlers( BVAppLogic* appLogic );
    ~SceneEventsHandlers();

    void        NodeStructure       ( bv::IEventPtr evt );
    void        PluginStructure     ( bv::IEventPtr evt );
    void        ProjectStructure    ( bv::IEventPtr evt );
    void        SceneStructure	    ( bv::IEventPtr evt );
    void        TimelineHandler     ( bv::IEventPtr evt );
    void        WidgetHandler       ( bv::IEventPtr evt );

	void        OnNodeAppearing     ( IEventPtr evt );
	void        OnNodeLeaving       ( IEventPtr evt );
	void        OnNoMoreNodes       ( IEventPtr evt );
};


} //bv
