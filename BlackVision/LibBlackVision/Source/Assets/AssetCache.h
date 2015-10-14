#pragma once

#include "Assets/Asset.h"
#include "Assets/AssetDescriptor.h"

#include <map>


namespace bv
{

class AssetCache
{
private:
	std::map< std::string, AssetConstPtr > m_assets;

private:
	AssetConstPtr					Find				( const std::string & key ) const;

public:
	AssetCache();
	~AssetCache();

	// **************************
	// Adds entry to cache if doesn't exist.
	bool							Add		( const std::string& key, const AssetConstPtr& resource );
	bool							Add		( const AssetDescConstPtr& descriptor, const AssetConstPtr& resource );


	// **************************
	// Adds entry to cache if doesn't exist or update if exists.
	void							Update	( const std::string& key, const AssetConstPtr& resource );

	bool							Exists	( const std::string& key ) const;
	bool							Exists	( const AssetDescConstPtr& descriptor ) const;

	AssetConstPtr					Get		( const std::string& key ) const;
	AssetConstPtr					Get		( const AssetDescConstPtr& descriptor ) const;
};


} //bv
