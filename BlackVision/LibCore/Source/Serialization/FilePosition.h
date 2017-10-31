#pragma once

#include "CoreDEF.h"



namespace bv
{

// ***********************
//
struct FilePosition
{
    SizeType    Line;
    SizeType    CharPosition;

    // ***********************
    //
    FilePosition()
        : Line( 0 )
        , CharPosition( 0 )
    {}
};



}	// bv

