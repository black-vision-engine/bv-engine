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
        : m_assets( nullptr )
    {
        recursive = true;
        detailedInfo = true;
        pluginsInfo = true;
        extendedAssetData = false;
    }

    bool            recursive;
    bool            detailedInfo;
    bool            pluginsInfo;
    bool            extendedAssetData;

    AssetDescsWithUIDsPtr               GetAssets               ();
    void                                SetAssets               ( const AssetDescsWithUIDsPtr & assets );
};

}
