#include "AssetCategory.h"
#include "AssetAccessor.h"

namespace bv
{

// ********************************
//
AssetCategoryConstPtr	AssetCategory::Create( const std::string & id, const AssetAccessorConstPtr & aa )
{
	return AssetCategoryConstPtr( new AssetCategory( id, aa ) );
}

// ********************************
//
AssetCategory::AssetCategory( const std::string & id, const AssetAccessorConstPtr & aa )
	: m_assetAccessor( aa )
	, m_id( id )
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
void					AssetCategory::AddAsset	( const Path & path, const AssetDescConstPtr & assetDesc ) const
{
	m_assetAccessor->AddAsset( path, assetDesc );
}

// ********************************
//
AssetDescConstPtr		AssetCategory::GetAssetDesc	( const Path & path ) const
{
	return m_assetAccessor->GetAssetDesc( path );
}

// ********************************
//
void					AssetCategory::RemoveAsset	( const Path & path ) const
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
void					AssetCategory::ExportAsset	( std::ostream & out, const Path & path ) const
{
	m_assetAccessor->ExportAsset( out, path );
}

// ********************************
//
void					AssetCategory::ImportAsset	( const Path & impAssetFilePath, const Path & toPath ) const
{
	m_assetAccessor->ImportAsset( impAssetFilePath, toPath );
}

// ********************************
//
void					AssetCategory::ImportAsset	( std::istream & in, const Path & toPath ) const
{
	m_assetAccessor->ImportAsset( in, toPath );
}

// ********************************
//
void					AssetCategory::ExportAll	( const Path & expAssetFilePath ) const
{
	m_assetAccessor->ExportAll( expAssetFilePath );
}

// ********************************
//
PathVec					AssetCategory::ListAssets	( const Path & path ) const
{
	return m_assetAccessor->ListAll( path );
}

} // bv