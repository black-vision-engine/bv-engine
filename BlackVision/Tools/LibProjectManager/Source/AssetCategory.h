#pragma once

#include "AssetAccessor.h"

#include "Assets/FwdDecls.h"

#include "System/Path.h"

namespace bv
{

class AssetCategory;
DEFINE_CONST_PTR_TYPE( AssetCategory )

class AssetCategory
{
private:

	std::string				m_id;
	AssetAccessorConstPtr	m_assetAccessor;

	explicit				AssetCategory	( const std::string & id, const AssetAccessorConstPtr & aa );

public:

	static AssetCategoryConstPtr	Create	( const std::string & id, const AssetAccessorConstPtr & aa );
							
							~AssetCategory	();

	const std::string &		GetId			() const;

	void					AddAsset		( const Path & path, const AssetDescConstPtr & assetDesc ) const;
	
	AssetDescConstPtr		GetAssetDesc	( const Path & path ) const;
	
	void					RemoveAsset		( const Path & path ) const;
	
	void					ExportAsset		( const Path & expAssetFilePath, const Path & path ) const;
	void					ExportAsset		( std::ostream & out, const Path &  path ) const;

	void					ImportAsset		( const Path & impAssetFilePath, const Path & toPath ) const;
	void					ImportAsset		( std::istream & in, const Path &  toPath ) const;
	
	void					ExportAll		( const Path & expAssetFilePath ) const;
	
	PathVec					ListAssets		( const Path & projectName ) const;

};

} // bv