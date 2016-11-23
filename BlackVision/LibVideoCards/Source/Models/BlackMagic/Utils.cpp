#include "Utils.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"


namespace bv { namespace videocards { namespace blackmagic {
  
//**************************************
//
std::map< std::size_t, BMDDisplayMode >    VideoModeMap = CreateVideoModeMap();

//**************************************
//
std::map< std::size_t, BMDDisplayMode >    CreateVideoModeMap  ()
{
    std::map< std::size_t, BMDDisplayMode > videoModeMap;

    videoModeMap[ VideoModeHash( 1080, 5000, true ) ] = BMDDisplayMode::bmdModeHD1080i50;
    videoModeMap[ VideoModeHash( 1080, 5994, true ) ] = BMDDisplayMode::bmdModeHD1080i5994;
    videoModeMap[ VideoModeHash( 1080, 6000, true ) ] = BMDDisplayMode::bmdModeHD1080i6000;

    videoModeMap[ VideoModeHash( 1080, 2397, false ) ] = BMDDisplayMode::bmdModeHD1080p2398;
    videoModeMap[ VideoModeHash( 1080, 2400, false ) ] = BMDDisplayMode::bmdModeHD1080p24;
    videoModeMap[ VideoModeHash( 1080, 2500, false ) ] = BMDDisplayMode::bmdModeHD1080p25;
    videoModeMap[ VideoModeHash( 1080, 2997, false ) ] = BMDDisplayMode::bmdModeHD1080p2997;
    videoModeMap[ VideoModeHash( 1080, 3000, false ) ] = BMDDisplayMode::bmdModeHD1080p30;
    videoModeMap[ VideoModeHash( 1080, 5000, false ) ] = BMDDisplayMode::bmdModeHD1080p50;
    videoModeMap[ VideoModeHash( 1080, 5994, false ) ] = BMDDisplayMode::bmdModeHD1080p5994;
    videoModeMap[ VideoModeHash( 1080, 6000, false ) ] = BMDDisplayMode::bmdModeHD1080p6000;
    videoModeMap[ VideoModeHash( 720, 5000, false ) ] = BMDDisplayMode::bmdModeHD720p50;
    videoModeMap[ VideoModeHash( 720, 5994, false ) ] = BMDDisplayMode::bmdModeHD720p5994;
    videoModeMap[ VideoModeHash( 720, 6000, false ) ] = BMDDisplayMode::bmdModeHD720p60;

    return videoModeMap;
}

//**************************************
//
BMDDisplayMode     ConvertVideoMode    ( UInt32 resolution, UInt32 refresh, bool interlaced )
{
    auto hash = VideoModeHash( resolution, refresh, interlaced );
    if( VideoModeMap.count( hash ) )
    {
        return VideoModeMap.at( hash );
    }

    assert( false ); //FIXME: should never reach that point
    return BMDDisplayMode::bmdModeHD1080i50;
}

//**************************************
//
std::size_t     VideoModeHash( UInt32 resolution, UInt32 refresh, bool interlaced ) {
    std::size_t seed = 0;
    std::hash< UInt32 > hasher;

    seed ^= hasher( resolution ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    seed ^= hasher( refresh ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    seed ^= hasher( interlaced ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );

    return seed;
}

} //blackmagic
} //videocards
} //bv