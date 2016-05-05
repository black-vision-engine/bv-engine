#include "VideoStreamAssetAccessor.h"

#include "Assets/VideoStream/VideoStreamAssetDescriptor.h"
#include "Assets/VideoStream/VideoStreamAsset.h"

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
VideoStreamAssetAccessorConstPtr VideoStreamAssetAccessor::Create( const Path & rootPath, const StringVector & fileExts )
{
	return VideoStreamAssetAccessorConstPtr( new VideoStreamAssetAccessor( rootPath, fileExts ) );
}

// ***********************
//
VideoStreamAssetAccessor::VideoStreamAssetAccessor( const Path & rootPath, const StringVector & fileExts )
	: m_rootPath( rootPath )
	, m_fileExts( fileExts )
{
	if( !Dir::Exists( m_rootPath.Str() ) )
	{
		Dir::CreateDir( m_rootPath.Str(), true );
	}
}

// ***********************
//
VideoStreamAssetAccessor::~VideoStreamAssetAccessor()
{}


// ********************************
//
AssetDescConstPtr	VideoStreamAssetAccessor::GetAssetDesc	( const Path & path ) const
{
	auto p = m_rootPath / path;

	if( Path::Exists( p ) )
	{
        return VideoStreamAssetDesc::Create( p.Str(), TextureFormat::F_A8R8G8B8 );
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Asset '" << p.Str() << "' doesn't exist.";
		return nullptr;
	}
}

// ********************************
//
void			 	VideoStreamAssetAccessor::AddAsset		( const Path & internalPath, const AssetDescConstPtr & assetDesc ) const
{
	auto uid = assetDesc->GetUID();

	if( uid == VideoStreamAssetDesc::UID() )
	{
		auto typedDesc = QueryTypedDesc< VideoStreamAssetDescConstPtr >( assetDesc );

        auto path = typedDesc->GetStreamPath();

		Path::Copy( path, m_rootPath / internalPath );
	}
	else
	{
		assert( !"Wrong asset descriptor type" );
	}
}

// ********************************
//
void			 	VideoStreamAssetAccessor::RemoveAsset	( const Path & internalPath ) const
{
	Path::Remove( m_rootPath / internalPath );
}

// ********************************
//
void			 	VideoStreamAssetAccessor::RenameAsset	( const Path & oldPath, const Path & newPath ) const
{
	Path::Rename( m_rootPath / oldPath, m_rootPath / newPath );
}

// ********************************
//
void			 	VideoStreamAssetAccessor::ExportAsset	( const Path & expAssetFilePath, const Path & internalPath) const
{
	auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMWriteAppend );

	auto out = expFile.StreamBuf();

	ExportAsset( *out, internalPath );

	expFile.Close();
}

// ********************************
//
void			 	VideoStreamAssetAccessor::ExportAsset	( std::ostream & out, const Path & internalPath) const
{
	auto absPath = m_rootPath / internalPath;

	if( Path::Exists( absPath ) && Path::IsFile( absPath ) )
	{
		auto assetFile = File::Open( absPath.Str(), File::OpenMode::FOMReadOnly );

		out << internalPath.Str();
        out << '\n';
		out << std::to_string( File::Size( absPath.Str() ) );
        out << '\n';

        assetFile.Read( out );

		assetFile.Close();
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannot export asset: " << absPath.Str();
	}
}

// ********************************
//
void			 	VideoStreamAssetAccessor::ImportAsset	( const Path & impAssetFile, const Path & importToPath ) const
{
	auto impAsset = File::Open( impAssetFile.Str(), File::OpenMode::FOMReadOnly );

	auto in = impAsset.StreamBuf();

	ImportAsset( *in, importToPath );
}

// ********************************
//
void				VideoStreamAssetAccessor::ImportAsset	( std::istream & in, const Path &  importToPath ) const
{
	auto absPath = m_rootPath / importToPath;

	auto assetFile = File::Open( absPath.Str(), File::OpenMode::FOMReadWrite );

	std::stringbuf buf;

    in.get( buf, '\n' );
    in.ignore();

    Path path = buf.str();

    buf.str("");
    in.get( buf, '\n' );
    in.ignore();

    SizeType size = stoul( buf.str() );

	assetFile.Write( in, size );

	assetFile.Close();
}

// ********************************
//
void			 	VideoStreamAssetAccessor::ExportAll		( std::ostream & out ) const
{
	for( auto p : ListAllUnique( m_rootPath ) )
	{
		ExportAsset( out, p );
	}
}

// ********************************
//
void			 	VideoStreamAssetAccessor::ExportAll		( const Path & expAssetFilePath ) const
{
	auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMWriteAppend );

	auto out = expFile.StreamBuf();

	ExportAll( *out );

	expFile.Close();
}

// ********************************
//
PathVec              VideoStreamAssetAccessor::ListAll		( const Path & path, bool recursive ) const
{
	PathVec ret;
	for( auto ext : m_fileExts )
	{
		auto l = Path::List( m_rootPath / path, recursive, ext );

        for( auto & p : l )
        {
            p = Path::RelativePath( p, m_rootPath );
        }

		ret.insert( ret.end(), l.begin(), l.end() );
	}
	
	return ret;
}

// ********************************
//
PathVec	VideoStreamAssetAccessor::ListAllUnique	( const Path & path ) const
{
	auto l = ListAll( path, true );

	std::set< Path  > unique;

	for( auto p : l )
	{
		unique.insert( p );
	}

	return PathVec( unique.begin(), unique.end() );	
}

// ********************************
//
UInt64 VideoStreamAssetAccessor::GetAssetSizeInBytes ( const Path & path ) const
{
    return File::GetSize( ( m_rootPath / path ).Str() );
}

}   // bv
