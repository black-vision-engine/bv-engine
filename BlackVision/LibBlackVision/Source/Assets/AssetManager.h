#pragma once

#include "AssetDescriptor.h"
#include "Assets/Asset.h"
#include "Assets/AssetLoader.h"

#include <map>

namespace bv
{

class ResourceManager
{
public:
	model::ResourceConstPtr			LoadResource	( const ResourceDescConstPtr & desc ) const;
	bool							RegisterLoader	( const std::string & resDescUID, model::ResourceLoader * loader );
	bool							UnregisterLoader( const std::string & resDescUID );

	static ResourceManager &		GetInstance		();

private:

	explicit						ResourceManager();
									~ResourceManager();

	std::map< std::string, model::ResourceLoader * > m_loaders;
};

} // bv