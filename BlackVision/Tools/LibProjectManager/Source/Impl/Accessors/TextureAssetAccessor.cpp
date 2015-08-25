#include "TextureAssetAccessor.h"
#include "Assets/Texture/SingleTextureAssetDescriptor.h"
#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/GeneratedSingleTextureAssetDescriptor.h"
#include "LibImage.h"
#include "Engine/Types/EnumsUtils.h"
#include "IO/DirIO.h"
#include "IO/FileIO.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibBlackVision

#include <set>
#include <fstream>
#include <cassert>

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
	auto uid = assetDesc->GetUID();

	if( uid == SingleTextureAssetDesc::UID() )
	{
		auto typedDesc = QueryTypedDesc< SingleTextureAssetDescConstPtr >( assetDesc );

		auto path = typedDesc->GetImagePath();

		Path::Copy( path, m_rootPath / internalPath );
	}
	else
	{
		assert( !"Wrong asset descriptor type" );
	}
}

// ********************************
//
void			 	TextureAssetAccessor::RemoveAsset	( const Path & internalPath )
{
	Path::Remove( m_rootPath / internalPath );
}

// ********************************
//
void			 	TextureAssetAccessor::RenameAsset	( const Path & oldPath, const Path & newPath )
{
	Path::Rename( m_rootPath / oldPath, m_rootPath / newPath );
}

// ********************************
//
void			 	TextureAssetAccessor::ExportAsset	( const Path & expAssetFilePath, const Path & internalPath) const
{
	auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMWriteAppend );

	auto out = expFile.StreamBuf();

	ExportAsset( *out, internalPath );

	expFile.Close();
}

// ********************************
//
void			 	TextureAssetAccessor::ExportAsset	( std::ostream & out, const Path & internalPath) const
{
	auto absPath = m_rootPath / internalPath;

	if( Path::Exists( absPath ) && Path::IsFile( absPath ) )
	{
		auto assetFile = File::Open( absPath.Str(), File::OpenMode::FOMReadOnly );

		out << internalPath.Str();
		out << File::Size( absPath.Str() );

		auto in = assetFile.StreamBuf();

		out << *in;

		assetFile.Close();
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannot export asset: " << absPath.Str();
	}
}

// ********************************
//
void			 	TextureAssetAccessor::ImportAsset	( const Path & impAssetFile, const Path & importToPath )
{
	auto impAsset = File::Open( impAssetFile.Str(), File::OpenMode::FOMReadOnly );

	auto in = impAsset.StreamBuf();

	ImportAsset( *in, importToPath );
}

// ********************************
//
void				TextureAssetAccessor::ImportAsset	( std::istream & in, const Path &  importToPath )
{
	auto absPath = m_rootPath / importToPath;

	auto assetFile = File::Open( absPath.Str(), File::OpenMode::FOMReadWrite );

	std::string filename;
	bv::SizeType fileSize;
	
	in >> filename;
	in >> fileSize;

	*assetFile.StreamBuf() << in;

	assetFile.Close();
}

// ********************************
//
void			 	TextureAssetAccessor::ExportAll		( std::ostream & out ) const
{
	for( auto p : ListAllUnique( m_rootPath ) )
	{
		ExportAsset( out, p );
	}
}

// ********************************
//
void			 	TextureAssetAccessor::ExportAll		( const Path & expAssetFilePath ) const
{
	auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMWriteAppend );

	auto out = expFile.StreamBuf();

	ExportAll( *out );

	expFile.Close();
}

// ********************************
//
PathVec	TextureAssetAccessor::ListAll		( const Path & path ) const
{
	PathVec ret;
	for( auto ext : m_fileExts )
	{
		auto l = Path::List( path, ext );
		ret.insert( ret.end(), l.begin(), l.end() );
	}
	
	return ret;
}

// ********************************
//
namespace
{

bool PathCompare( const Path & a, const Path & b ) 
{
    return a.Str() < b.Str();
}

}

// ********************************
//
PathVec	TextureAssetAccessor::ListAllUnique	( const Path & path ) const
{
	auto l = ListAll( path );

	std::set< Path, bool (*)( const Path & a, const Path & b )  > unique( PathCompare );

	for( auto p : l )
	{
		unique.insert( p );
	}

	return PathVec( unique.begin(), unique.end() );	
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