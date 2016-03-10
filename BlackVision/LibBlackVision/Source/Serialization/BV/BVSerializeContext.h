#pragma once

#include "Serialization/SerializeContext.h"
#include "Assets/AssetDescsWithUIDs.h"

#include "CoreDEF.h"

namespace bv {


class BVSerializeContext : public SerializeContext
{
private:

    AssetDescsWithUIDsPtr               m_assets;

public:
    BVSerializeContext()
        : m_assets( std::make_shared< AssetDescsWithUIDs >() )
    {
        recursive = true;
        detailedInfo = true;
        pluginsInfo = true;
        extendedAssetData = false;
        sceneNameInTimeline = true;
    }

    bool            recursive;
    bool            detailedInfo;
    bool            pluginsInfo;
    bool            extendedAssetData;
    bool            sceneNameInTimeline;

    AssetDescsWithUIDsPtr               GetAssets               ();
    void                                SetAssets               ( const AssetDescsWithUIDsPtr & assets );
};

}
