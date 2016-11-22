#pragma once

#include <map>

#include "VideoCardManagerUtils.h"
#include "DeckLinkAPI_h.h"

#include "CoreDEF.h"


namespace bv { namespace videocards { namespace blackmagic {


//**************************************
//
struct ChannelInputData
{
    IOType                  type;
    bool                    playthrough;
};

DEFINE_UPTR_TYPE( ChannelInputData )


//**************************************
//
struct ChannelOutputData
{
    IOType                  type;
    UInt32                  resolution;
    UInt32                  refresh;
    bool                    interlaced;
    bool                    flipped;
    BMDDisplayMode          videoMode;
};

DEFINE_UPTR_TYPE( ChannelOutputData )


//**************************************
//
BMDDisplayMode                                          ConvertVideoMode        ( UInt32 resolution, UInt32 refresh, bool interlaced );
std::size_t                                             VideoModeHash           ( UInt32 resolution, UInt32 refresh, bool interlaced );
std::map< std::size_t, BMDDisplayMode >                 CreateVideoModeMap      ();
extern  std::map< std::size_t, BMDDisplayMode >         VideoModeMap;

} //blackmagic
} //videocards
} //bv

