#pragma once

#include "Assets/Assets.h"


namespace bv {


struct AVAssetUtils
{

    static ThumbnailConstPtr            LoadThumbnail       ( const AVAssetDescConstPtr & desc );
    static SingleTextureAssetConstPtr   LoadSingleFrame     ( const AVAssetDescConstPtr & desc, TimeType frameTime );

};

}   // bv
