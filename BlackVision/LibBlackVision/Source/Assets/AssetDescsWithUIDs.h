#pragma once

#include <map>

#include "AssetDescriptor.h"
#include "AssetManager.h"

//#include "Engine/Models/BasicNode.h" // FIXME this is only important for GetAssetsWithUIDs

namespace bv {

class AssetDescsWithUIDs;
DEFINE_PTR_TYPE( AssetDescsWithUIDs );

class AssetDescsWithUIDs : public ISerializable
{
    static AssetDescsWithUIDs instance;

    std::map< std::string, AssetDescConstPtr >              m_uid2asset;
    std::map< std::string, std::string >                    m_key2uid;

    void                                                    AddAssetDescWithUID( AssetDescConstPtr desc, std::string uid );

public:

    //static AssetDescsWithUIDs &                             GetInstance() { return instance; }
    //static void                                             SetInstance( AssetDescsWithUIDs & i ) { instance = i; }

    virtual void                                            Serialize       ( ISerializer & ser ) const;
    static AssetDescsWithUIDsPtr                            Create          ( const IDeserializer & deser );
    static AssetDescsWithUIDsPtr                            Create          ();

    void                                                    AddAssetDesc( AssetDescConstPtr asset );

    AssetDescVec                                            GetAssetsDescs() const;

    std::string                                             Key2UID( std::string key ) { return m_key2uid[ key ]; }
    AssetDescConstPtr                                       UID2Asset( std::string uid ) { return m_uid2asset[ uid ]; }
};

DEFINE_PTR_TYPE( AssetDescsWithUIDs );

namespace model {
class BasicNode;
class IPlugin;
}

void GetAssetsWithUIDs( AssetDescsWithUIDs& map, const model::BasicNode * root, bool recursive = false );
void GetAssetsWithUIDs( AssetDescsWithUIDs& map, const model::IPlugin * plugin );


} // bv