#pragma once

#include <map>

#include "AssetDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetCache.h"


namespace bv
{

class AssetManager
{
private:

	std::map< std::string, AssetLoaderConstPtr >	m_loaders;
	AssetCache										m_assetCache;

public:

    AssetDescConstPtr				CreateDesc		( const IDeserializer & deserializer );
	AssetConstPtr					LoadAsset		( const AssetDescConstPtr & desc );
	bool							RegisterLoader	( const std::string & assetDescUID, const AssetLoaderConstPtr & loader );
	bool							UnregisterLoader( const std::string & assetDescUID );

    void                            AddToCache      ( AssetDescConstPtr & desc, AssetConstPtr asset );
    AssetConstPtr                   GetFromCache    ( AssetDescConstPtr & desc );

	static AssetManager &			GetInstance		();

private:

	explicit						AssetManager();
									~AssetManager();

	void							RegisterBasicLoaders();

};

// ***********************
//
template< typename AssetType >
std::shared_ptr< const AssetType > LoadTypedAsset		( const AssetDescConstPtr & desc )
{
    if( desc->GetUID() == GetAssetDescUID<AssetType>() )
	{
		auto asset = AssetManager::GetInstance().LoadAsset( desc );
		return std::static_pointer_cast<const AssetType>( asset );
	}
	return nullptr;
}



} // bv