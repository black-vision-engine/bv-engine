#include "BVSerializeContext.h"

#include "Assets/AssetDescsWithUIDs.h"

namespace bv {

// ***********************
//
AssetDescsWithUIDsPtr               BVSerializeContext::GetAssets               ()
{
    return m_assets;
}

// ***********************
//
void                                BVSerializeContext::SetAssets               ( const AssetDescsWithUIDsPtr & assets )
{
    m_assets = assets;
}

}
