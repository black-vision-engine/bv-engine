#pragma once

#include "Serialization/SerializeContext.h"

#include "CoreDEF.h"

namespace bv {


// FORWARD DECLARATIONS
class AssetDescsWithUIDs;
DEFINE_PTR_TYPE( AssetDescsWithUIDs );    

class BVSerializeContext : public SerializeContext
{
    AssetDescsWithUIDsPtr               m_assets;

public:
    BVSerializeContext()
        : m_assets( nullptr )
    {
        recursive = true;
        detailedInfo = true;
        pluginsInfo = true;
    }

    bool            recursive;
    bool            detailedInfo;
    bool            pluginsInfo;

    AssetDescsWithUIDsPtr               GetAssets               ();
    void                                SetAssets               ( const AssetDescsWithUIDsPtr & assets );
};

}
