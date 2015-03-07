#pragma once

#include "ResourceDescriptor.h"
#include "Engine/Models/Resources/IResource.h"
#include "Engine/Models/Resources/ResourceLoader.h"

#include <map>

namespace bv
{

class ResourceManager
{
public:
	model::IResourceConstPtr		LoadResource	( const ResourceDescConstPtr & desc ) const;
	bool							RegisterLoader	( const std::string & resDescUID, model::ResourceLoader * loader );
	bool							UnregisterLoader( const std::string & resDescUID );

	static ResourceManager &		GetInstance		();

private:

	explicit						ResourceManager();
									~ResourceManager();

	std::map< std::string, model::ResourceLoader * > m_loaders;
};

} // bv