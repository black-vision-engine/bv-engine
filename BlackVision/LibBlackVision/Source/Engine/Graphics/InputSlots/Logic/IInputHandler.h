#pragma once

#include "CoreDEF.h"

#include "Engine/Graphics/InputSlots/InputSlots.h"


namespace bv
{

// ***********************
//
class IInputHandler
{
private:
public:

    virtual     ~IInputHandler  () = 0 {}

    virtual void        RegisterInputs      ( InputSlotsPtr inputSlots )    = 0;
    virtual void        UnregisterInputs    ()                              = 0;

    virtual void        ProcessInputs       ()                              = 0;
};

DEFINE_PTR_TYPE( IInputHandler )


}	// bv
