#pragma once


#include "CoreDEF.h"

#include <string>


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

    std::string     ImagesDirectory;        ///< Images from this directory will be sent as input.
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
