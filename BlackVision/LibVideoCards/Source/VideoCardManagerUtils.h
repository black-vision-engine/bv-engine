#pragma once



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

