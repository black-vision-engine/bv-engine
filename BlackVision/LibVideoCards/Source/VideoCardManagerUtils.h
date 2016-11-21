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

struct ChannelInputData
{
    IOType                  type;
    bool                    playthrough;
};

DEFINE_UPTR_TYPE( ChannelInputData )

struct ChannelOutputData
{
    IOType                  type;
    UInt32                  resolution;
    UInt32                  refresh;
    bool                    interlaced;
    bool                    flipped;
    _EVideoMode             videoMode;
    _EBlueGenlockSource     referenceMode;
    Int32                   referenceH;
    Int32                   referenceV;
};

DEFINE_UPTR_TYPE( ChannelOutputData )

} //videocards
} //bv

