#include "AssetCategory.h"
#include "AssetAccessor.h"

namespace bv
{

// ********************************
//
AssetCategory::AssetCategory( AssetAccessor * aa )
	: m_assetAccessor( aa )
{}

// ********************************
//
AssetCategory::~AssetCategory()
{}

// ********************************
//
const std::string &		AssetCategory::GetId() const
{
	return m_id;
}

// ********************************
//
void					AssetCategory::AddAsset	( const NormalizedPath & path, const AssetDesc & assetDesc )
{
	m_assetAccessor->AddAsset( path, assetDesc );
}

// ********************************
//
const AssetDesc *		AssetCategory::GetAssetDesc	( const NormalizedPath & path ) const
{
	return m_assetAccessor->GetAssetDesc( path );
}

// ********************************
//
void					AssetCategory::RemoveAsset	( const NormalizedPath & path )
{
	m_assetAccessor->RemoveAsset( path );
}

// ********************************
//
void					AssetCategory::ExportAsset	( const NormalizedPath & expAssetFilePath, const NormalizedPath & path ) const
{
	m_assetAccessor->ExportAsset( expAssetFilePath, path );
}

// ********************************
//
void					AssetCategory::ImportAsset	( const NormalizedPath & impAssetFilePath, const NormalizedPath & toPath )
{
	m_assetAccessor->ImportAsset( impAssetFilePath, toPath );
}

// ********************************
//
void					AssetCategory::ExportAll	( const NormalizedPath & expAssetFilePath ) const
{
	m_assetAccessor->ExportAll( expAssetFilePath );
}

// ********************************
//
void					AssetCategory::ListAssets	( const NormalizedPath & projectName ) const
{
	m_assetAccessor->ListAll( projectName );
}

} // bv