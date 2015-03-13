#pragma once

#include "AssetDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/AssetLoader.h"

#include <map>

namespace bv
{

class AssetManager
{
public:
	AssetConstPtr					LoadAsset		( const AssetDescConstPtr & desc ) const;
	bool							RegisterLoader	( const std::string & assetDescUID, const AssetLoaderConstPtr & loader );
	bool							UnregisterLoader( const std::string & assetDescUID );

	static AssetManager &			GetInstance		();

private:

	explicit						AssetManager();
									~AssetManager();

	std::map< std::string, AssetLoaderConstPtr > m_loaders;

	void							RegisterBasicLoaders();
};

} // bv