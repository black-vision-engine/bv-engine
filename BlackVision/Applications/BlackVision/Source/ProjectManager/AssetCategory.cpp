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
void					AssetCategory::AddAsset	( const Path & path, const AssetDesc & assetDesc )
{
	m_assetAccessor->AddAsset( path, assetDesc );
}

// ********************************
//
const AssetDesc *		AssetCategory::GetAssetDesc	( const Path & path ) const
{
	return m_assetAccessor->GetAssetDesc( path );
}

// ********************************
//
void					AssetCategory::RemoveAsset	( const Path & path )
{
	m_assetAccessor->RemoveAsset( path );
}

// ********************************
//
void					AssetCategory::ExportAsset	( const Path & expAssetFilePath, const Path & path ) const
{
	m_assetAccessor->ExportAsset( expAssetFilePath, path );
}

// ********************************
//
void					AssetCategory::ImportAsset	( const Path & impAssetFilePath, const Path & toPath )
{
	m_assetAccessor->ImportAsset( impAssetFilePath, toPath );
}

// ********************************
//
void					AssetCategory::ExportAll	( const Path & expAssetFilePath ) const
{
	m_assetAccessor->ExportAll( expAssetFilePath );
}

// ********************************
//
void					AssetCategory::ListAssets	( const Path & projectName ) const
{
	m_assetAccessor->ListAll( projectName );
}

} // bv