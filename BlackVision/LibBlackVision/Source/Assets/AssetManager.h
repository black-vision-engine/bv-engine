#pragma once

#include "AssetDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/AssetLoader.h"
#include "Assets/AssetCache.h"

#include <map>

namespace bv
{

class AssetManager
{
public:
	AssetDescConstPtr				CreateDesc		( const std::string& jsonString );
	AssetConstPtr					LoadAsset		( const AssetDescConstPtr & desc );
	bool							RegisterLoader	( const std::string & assetDescUID, const AssetLoaderConstPtr & loader );
	bool							UnregisterLoader( const std::string & assetDescUID );



	static AssetManager &			GetInstance		();

private:

	explicit						AssetManager();
									~AssetManager();

	std::map< std::string, AssetLoaderConstPtr >	m_loaders;
	AssetCache										m_assetCache;

	void							RegisterBasicLoaders();

public:
};

// ***********************
//
template<typename AssetType>
std::shared_ptr<const AssetType> LoadTypedAsset		(  const AssetDescConstPtr & )
{
	return nullptr;
}



} // bv