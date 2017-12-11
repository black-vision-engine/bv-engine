#pragma once

#include "Engine/Events/Events.h"


namespace bv
{

class BVAppLogic;

class GenericEventsHandlers
{
private:

    BVAppLogic*         m_appLogic;

public:

    GenericEventsHandlers			( BVAppLogic* appLogic );
    ~GenericEventsHandlers			();

    void        EventHandler        ( bv::IEventPtr evt );

    void        SetWeightCenterHandler      ( const std::string & command, JsonSerializeObject & ser, IDeserializer * request, int eventID );
    void        SetNodeScaleHandler         ( const std::string & command, JsonSerializeObject & ser, IDeserializer * request, int eventID );

    void        DebugEventsCount            ( const std::string & command, JsonSerializeObject & ser, IDeserializer * request, int eventID );
};

} //bv
