#pragma once

#include <map>

#include "win_sock.h"
#include "BlueFish/Inc/BlueVelvet4.h"

#include "CoreDEF.h"


namespace bv { namespace videocards {

//**************************************
//
enum class IOType: int
{
    FILL_KEY,
    FILL,
    KEY,
    INVALID
};

enum class ReferenceMode : int 
{
    FREERUN = 0,
    IN_A,
    IN_B,
    ANALOG,
    GENLOCK
};

} //videocards
} //bv

