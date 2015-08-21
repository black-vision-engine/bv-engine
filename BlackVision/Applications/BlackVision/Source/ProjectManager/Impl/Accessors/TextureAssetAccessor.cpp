#include "TextureAssetAccessor.h"

namespace bv
{

// ********************************
//
TextureAssetAccessor::TextureAssetAccessor				( const ProjectManager * projectManager, const NormalizedPath & rootPath, const StringVector & fileExts )
	: m_projectManager( projectManager )
	, m_rootPath( rootPath )
	, m_fileExts( fileExts )
{}

// ********************************
//
TextureAssetAccessor::~TextureAssetAccessor				()
{}

// ********************************
//
const AssetDesc *	TextureAssetAccessor::GetAssetDesc	( const NormalizedPath & path ) const
{

}

// ********************************
//
void			 	TextureAssetAccessor::AddAsset		( const NormalizedPath & internalPath, const AssetDesc & assetDesc )
{
}

// ********************************
//
void			 	TextureAssetAccessor::RemoveAsset	( const NormalizedPath & internalPath )
{
}

// ********************************
//
void			 	TextureAssetAccessor::RenameAsset	( const NormalizedPath & oldPath, const NormalizedPath & newPath )
{
}

// ********************************
//
void			 	TextureAssetAccessor::ImportAsset	( const NormalizedPath & impAssetFile, const NormalizedPath &  importToPath )
{
}

// ********************************
//
void			 	TextureAssetAccessor::ExportAsset	( const NormalizedPath & expAssetFilePath, const NormalizedPath &  internalPath) const
{
}

// ********************************
//
void			 	TextureAssetAccessor::ExportAll		( const NormalizedPath & expAssetFilePath ) const
{
}

// ********************************
//
NormalizedPathVec	TextureAssetAccessor::ListAll		( const NormalizedPath & path ) const
{
}

// ********************************
//
NormalizedPathVec	TextureAssetAccessor::ListAllUnique	( const NormalizedPath & path ) const
{
}

} // bv