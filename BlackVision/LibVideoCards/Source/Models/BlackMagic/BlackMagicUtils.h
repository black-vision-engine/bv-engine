#pragma once

#include <map>

#include "VideoCardManagerUtils.h"

#define _WINSOCKAPI_
#include "DeckLinkAPI_h.h"

#include "CoreDEF.h"

#define SUCCESS( hr ) ( ( ( HRESULT )( hr ) ) == S_OK )

namespace bv { namespace videocards { namespace blackmagic {


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
	bool					enabled;
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

