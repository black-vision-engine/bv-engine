#include "MeshAssetAccessor.h"

#include "Assets/Mesh/MeshAssetDescriptor.h"
#include "Assets/Mesh/MeshAsset.h"

#include "IO/DirIO.h"
#include "IO/FileIO.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibBlackVision

#include <set>
#include <fstream>


namespace bv
{


// ********************************
//
MeshAssetAccessorConstPtr MeshAssetAccessor::Create ( const Path & rootPath, const StringVector & fileExts )
{
    struct make_shared_enabler_MeshAssetAccessor : public MeshAssetAccessor
    {
        make_shared_enabler_MeshAssetAccessor( const Path & rootPath, const StringVector & fileExts )
            : MeshAssetAccessor( rootPath, fileExts )
        {
        }
    };

    return std::make_shared< make_shared_enabler_MeshAssetAccessor >( rootPath, fileExts );
}

// ***********************
//
MeshAssetAccessor::MeshAssetAccessor                ( const Path & rootPath, const StringVector & fileExts )
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
MeshAssetAccessor::~MeshAssetAccessor               ()
{
}

// ********************************
//
AssetDescConstPtr	MeshAssetAccessor::GetAssetDesc	    ( const Path & path ) const
{
	auto p = m_rootPath / path;

	if( Path::Exists( p ) )
	{
        return MeshAssetDesc::Create( ( Path( "meshes" ) / path ).Str(), "", true );
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::warning ) << "Asset '" << p.Str() << "' doesn't exist.";
		return nullptr;
	}
}

// ********************************
//
void			 	MeshAssetAccessor::AddAsset		    ( const Path & internalPath, const AssetDescConstPtr & assetDesc ) const
{
	auto uid = assetDesc->GetUID();

	if( uid == MeshAssetDesc::UID() )
	{
		auto typedDesc = QueryTypedDesc< MeshAssetDescConstPtr >( assetDesc );

        auto path = typedDesc->GetPath();

		Path::Copy( path, m_rootPath / internalPath );
	}
	else
	{
		assert( !"Wrong asset descriptor type" );
	}
}

// ********************************
//
void			 	MeshAssetAccessor::RemoveAsset	    ( const Path & internalPath ) const
{
	Path::Remove( m_rootPath / internalPath );
}

// ********************************
//
void			 	MeshAssetAccessor::RenameAsset	    ( const Path & oldPath, const Path & newPath ) const
{
	Path::Rename( m_rootPath / oldPath, m_rootPath / newPath );
}

// ********************************
//
void			 	MeshAssetAccessor::ExportAsset	( const Path & expAssetFilePath, const Path & internalPath) const
{
	auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMWriteAppend );

	auto out = expFile.StreamBuf();

	ExportAsset( *out, internalPath );

	expFile.Close();
}

// ********************************
//
void			 	MeshAssetAccessor::ExportAsset	    ( std::ostream & out, const Path & internalPath) const
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
void			 	MeshAssetAccessor::ImportAsset	    ( const Path & impAssetFile, const Path & importToPath ) const
{
	auto impAsset = File::Open( impAssetFile.Str(), File::OpenMode::FOMReadOnly );

	auto in = impAsset.StreamBuf();

	ImportAsset( *in, importToPath );
}

// ********************************
//
void				MeshAssetAccessor::ImportAsset	    ( std::istream & in, const Path &  importToPath ) const
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
void			 	MeshAssetAccessor::ExportAll		( std::ostream & out ) const
{
	for( auto p : ListAllUnique( m_rootPath ) )
	{
		ExportAsset( out, p );
	}
}

// ********************************
//
void			 	MeshAssetAccessor::ExportAll		( const Path & expAssetFilePath ) const
{
	auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMWriteAppend );

	auto out = expFile.StreamBuf();

	ExportAll( *out );

	expFile.Close();
}

// ********************************
//
PathVec              MeshAssetAccessor::ListAll		    ( const Path & path, bool recursive ) const
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
PathVec	            MeshAssetAccessor::ListAllUnique    ( const Path & path ) const
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
UInt64              MeshAssetAccessor::GetAssetSizeInBytes ( const Path & path ) const
{
    return File::GetSize( ( m_rootPath / path ).Str() );
}

}   // bv
