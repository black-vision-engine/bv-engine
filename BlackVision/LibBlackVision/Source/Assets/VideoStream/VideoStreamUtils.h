#pragma once

#include "Assets/Assets.h"


namespace bv
{


struct VideoStreamUtils
{
    static ThumbnailConstPtr            LoadThumbnail       ( const VideoStreamAssetDescConstPtr & desc );
    static SingleTextureAssetConstPtr   LoadSingleFrame     ( const VideoStreamAssetDescConstPtr & desc, TimeType frameTime );
};


}   // bv
