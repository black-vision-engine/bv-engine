#pragma once

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
