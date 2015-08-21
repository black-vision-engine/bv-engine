#include "TextureAssetAccessor.h"
#include "Assets/Texture/SingleTextureAssetDescriptor.h"
#include "LibImage.h"
#include "Engine/Types/EnumsUtils.h"
#include "IO/DirIO.h"

namespace bv
{

// ********************************
//
TextureAssetAccessor::TextureAssetAccessor				( const ProjectManager * projectManager, const Path & rootPath, const StringVector & fileExts )
	: m_projectManager( projectManager )
	, m_rootPath( rootPath )
	, m_fileExts( fileExts )
{
	CreateDir();
}

// ********************************
//
TextureAssetAccessor::~TextureAssetAccessor				()
{}

// ********************************
//
AssetDescConstPtr	TextureAssetAccessor::GetAssetDesc	( const Path & path ) const
{
	auto p = m_rootPath / path;

	auto props = image::GetImageProps( p.Str() );

	return SingleTextureAssetDesc::Create( p.Str(), props.width, props.height, EnumsUtils::Convert( props.format ), true );
}

// ********************************
//
void			 	TextureAssetAccessor::AddAsset		( const Path & internalPath, const AssetDescConstPtr & assetDesc )
{
}

// ********************************
//
void			 	TextureAssetAccessor::RemoveAsset	( const Path & internalPath )
{
}

// ********************************
//
void			 	TextureAssetAccessor::RenameAsset	( const Path & oldPath, const Path & newPath )
{
}

// ********************************
//
void			 	TextureAssetAccessor::ImportAsset	( const Path & impAssetFile, const Path &  importToPath )
{
}

// ********************************
//
void			 	TextureAssetAccessor::ExportAsset	( const Path & expAssetFilePath, const Path &  internalPath) const
{
}

// ********************************
//
void			 	TextureAssetAccessor::ExportAll		( const Path & expAssetFilePath ) const
{
}

// ********************************
//
PathVec	TextureAssetAccessor::ListAll		( const Path & path ) const
{
		return PathVec();
}

// ********************************
//
PathVec	TextureAssetAccessor::ListAllUnique	( const Path & path ) const
{
			return PathVec();
}

// ********************************
//
void				TextureAssetAccessor::CreateDir		() const
{
	if( !Dir::Exists( m_rootPath.Str() ) )
	{
		Dir::CreateDir( m_rootPath.Str(), true );
	}
}

} // bv