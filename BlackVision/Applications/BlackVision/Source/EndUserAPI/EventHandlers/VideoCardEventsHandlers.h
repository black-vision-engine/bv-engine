#pragma once

#include "Engine/Events/Events.h"


namespace bv {

class BVAppLogic;

class VideoCardEventsHandlers
{
private:

    BVAppLogic *    m_appLogic;

public:

				    VideoCardEventsHandlers     ( BVAppLogic * appLogic );
				    ~VideoCardEventsHandlers    ();

    void            EventHandler			    ( IEventPtr evt );

};

} //bv
