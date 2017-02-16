#include "stdafx.h"

#include "VideoOutputsHandler.h"


namespace bv { namespace nrl {

// *********************************
//
void    VideoOutputsHandler::HandleOutputsData   ( const AVOutputsData & outputs )
{
    { outputs; }
}

// *********************************
//
void    VideoOutputsHandler::PreInitialize      ( OutputStaticDataVec & uniqueOutputSetups, const UintUintMapping & mapping )
{
    { uniqueOutputSetups; }
    { mapping; }
}

} //bv
} //nrl
