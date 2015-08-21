#pragma once
#include "NormalizedPath.h"

namespace bv
{

class AssetDesc;
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

	void					AddAsset		( const NormalizedPath & path, const AssetDesc & assetDesc );
	const AssetDesc *		GetAssetDesc	( const NormalizedPath & path ) const;
	void					RemoveAsset		( const NormalizedPath & path );
	void					ExportAsset		( const NormalizedPath & expAssetFilePath, const NormalizedPath & path ) const;
	void					ImportAsset		( const NormalizedPath & impAssetFilePath, const NormalizedPath & toPath );
	void					ExportAll		( const NormalizedPath & expAssetFilePath ) const;
	void					ListAssets		( const NormalizedPath & projectName ) const;

};

} // bv