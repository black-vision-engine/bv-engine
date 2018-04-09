#pragma once

#include "Engine/Events/Events.h"
#include "Interfaces/IVideoCard.h"


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

private:

    void            GetReferenceSignalInfoHandler   ( videocards::VideoCardManager * videoCardManager, VideoCardEventPtr videocardEvent );
    void            IsLockedHandler                 ( videocards::VideoCardManager * videoCardManager, VideoCardEventPtr videocardEvent );

private:

    Expected< videocards::IVideoCardPtr >       GetVideoCard        ( videocards::VideoCardManager * videoCardManager, UInt32 videoCardID );

};

} //bv
