#pragma once

#include "Serialization/ConversionHelper.h"


#define FRAME_BUFFER_SIZE 1

namespace bv {
namespace videocards
{


//**************************************
//
enum class IOType : int
{
    FILL_KEY,
    FILL,
    KEY,
    INVALID
};

// ***********************
//
enum class ReferenceMode : int 
{
    FreeRun,
    AnalogBlackBurst,
    AnalogTriLevel,
    DigitalInput1,
    DigitalInput2,
    Interlock,
    AnalogBlackBurstAux,
    AnalogTriLevelAux,

    FailMode            ///< Wrong ReferenceMode
};

// ***********************
//
struct ReferenceInfo
{
    ReferenceMode       Mode;
    UInt32              ReferenceH;
    UInt32              ReferenceV;
};

} //videocards

DECLARE_ENUM_SERIALIZATION( videocards::ReferenceMode )


} //bv

