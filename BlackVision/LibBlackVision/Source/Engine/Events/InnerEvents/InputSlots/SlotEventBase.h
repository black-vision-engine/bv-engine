#pragma once

#include "Engine/Events/BaseEvent.h"
#include "Engine/Models/BasicNode.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"


namespace bv
{


// *******************************
/// Base event for InputSlots notifications.
class SlotEventBase : public BaseEvent
{
public:
    explicit        SlotEventBase( SlotIndex idx, const std::string & name )
        :   Index( idx )
        ,   Name( name )
    {}

    
    SlotIndex           Index;
    std::string         Name;

};

DEFINE_PTR_TYPE( SlotEventBase );


}   // bv

