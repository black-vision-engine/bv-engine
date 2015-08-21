#pragma once

#include "Assets/FwdDecls.h"

#include "System/Path.h"

namespace bv
{

class AssetAccessor;

class AssetCategory
{
private:

	std::string				m_id;
	AssetAccessor *			m_assetAccessor;

public:

	explicit				AssetCategory	( AssetAccessor * aa );
							~AssetCategory	();

	const std::string &		GetId			() const;

	void					AddAsset		( const Path & path, const AssetDescConstPtr & assetDesc );
	
	AssetDescConstPtr		GetAssetDesc	( const Path & path ) const;
	
	void					RemoveAsset		( const Path & path );
	
	void					ExportAsset		( const Path & expAssetFilePath, const Path & path ) const;
	
	void					ImportAsset		( const Path & impAssetFilePath, const Path & toPath );
	
	void					ExportAll		( const Path & expAssetFilePath ) const;
	
	void					ListAssets		( const Path & projectName ) const;

};

} // bv