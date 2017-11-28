#pragma once


#include "CoreDEF.h"


namespace bv {
namespace videocards
{


// ***********************
//
struct FakeInputChannelData
{
    UInt32          LinkedVideoInput;

    UInt32          Width;
    UInt32          Height;
};

DEFINE_UPTR_TYPE( FakeInputChannelData )



// ***********************
//
struct FakeOutputChannelData
{
    UInt32          LinkedVideoOutput;

    UInt32          Width;
    UInt32          Height;
};

DEFINE_UPTR_TYPE( FakeOutputChannelData )



}   // videocards
}   // bv
