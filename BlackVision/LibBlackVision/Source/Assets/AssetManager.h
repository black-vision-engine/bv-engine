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
	model::AssetConstPtr			LoadAsset	( const AssetDescConstPtr & desc ) const;
	bool							RegisterLoader	( const std::string & assetDescUID, model::AssetLoader * loader );
	bool							UnregisterLoader( const std::string & assetDescUID );

	static AssetManager &		GetInstance		();

private:

	explicit						AssetManager();
									~AssetManager();

	std::map< std::string, model::AssetLoader * > m_loaders;
};

} // bv