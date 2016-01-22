#include "AnimationAssetAccessor.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"

#include "IO/FileIO.h"
#include "IO/DirIO.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibBlackVision

#include <set>
#include <fstream>

namespace bv
{

// ********************************
//
AnimationAssetAccessor::AnimationAssetAccessor( const Path & rootPath, const StringVector & fileExts )
    : m_rootPath( rootPath )
    , m_fileExts( fileExts )
{
    CreateDir();
}

// ********************************
//
AnimationAssetAccessor::~AnimationAssetAccessor()
{}

// ********************************
//
AnimationAssetAccessorConstPtr AnimationAssetAccessor::Create( const Path & rootPath, const StringVector & fileExts )
{
    return AnimationAssetAccessorConstPtr( new AnimationAssetAccessor( rootPath, fileExts ) );
}

// ********************************
//
AssetDescConstPtr AnimationAssetAccessor::GetAssetDesc( const Path & path ) const
{
    if( Path::Exists( m_rootPath / path ) )
    {
        for( auto fe : m_fileExts )
        {
            auto numFrames = Path::List( m_rootPath / path, false, fe ).size();
            if( numFrames > 0 )
            {
                return AnimationAssetDesc::Create( ( Path( "sequences" ) / path ).Str(), numFrames, fe );
            }
        }
    }

    LOG_MESSAGE( SeverityLevel::warning ) << "Asset '" << path << "' doesn't exist.";
    
    return nullptr;
}

// ********************************
//
void				AnimationAssetAccessor::AddAsset( const Path & internalPath, const AssetDescConstPtr & assetDesc ) const
{
    auto uid = assetDesc->GetUID();

	if( uid == AnimationAssetDesc::UID() )
	{
		auto typedDesc = QueryTypedDesc< AnimationAssetDescConstPtr >( assetDesc );

        auto files = Dir::ListFiles( typedDesc->GetPath(), typedDesc->GetFilter() );

        TextureAssetDescVec framesDesc;

        if ( files.size() > 0 )
        {
	        for( auto f : files )
	        {
                framesDesc.push_back( TextureAssetDesc::Create( f, true ) );
            }
        }

        for( auto f : framesDesc )
        {
            auto path = f->GetOrigTextureDesc()->GetImagePath();
            Path::Copy( path, m_rootPath / internalPath );
        }
	}
	else
	{
		assert( !"Wrong asset descriptor type" );
	}
}

// ********************************
//
void				AnimationAssetAccessor::RemoveAsset			( const Path & internalPath ) const
{
    Path::Remove( m_rootPath / internalPath );
}

// ********************************
//
void				AnimationAssetAccessor::RenameAsset	        ( const Path & oldPath, const Path & newPath) const
{
    Path::Copy( oldPath, newPath );
    Path::Remove( oldPath );
}

// ********************************
//
void				AnimationAssetAccessor::ImportAsset			( const Path & impAssetFile, const Path &  importToPath ) const
{
	auto impAsset = File::Open( impAssetFile.Str(), File::OpenMode::FOMReadOnly );

	ImportAsset( *impAsset.StreamBuf(), importToPath );
}

// ********************************
//
void				AnimationAssetAccessor::ImportAsset			( std::istream & in, const Path &  importToPath ) const
{
    auto absPath = m_rootPath / importToPath;

	Dir::CreateDir( absPath.Str(), true );

    std::stringbuf buf;
    in.get( buf, '\n' );
    in.ignore();

    Path internalPath = buf.str();
    buf.str( "" );

    in.get( buf, '\n' );
    in.ignore();

    auto numFrames = stoul( buf.str() );
    buf.str( "" );

    for( SizeType i = 0; i < numFrames; ++i )
    {
        in.get( buf, '\n' );
        in.ignore();

        auto frame = buf.str();
        buf.str( "" );
                
        in.get( buf, '\n' );
        in.ignore();
        auto size = stoul( buf.str() );

        auto assetFile = File::Open( ( absPath / frame ).Str(), File::OpenMode::FOMReadWrite );

        assetFile.Write( in, size );

        assetFile.Close();
    }
}

// ********************************
//
void				AnimationAssetAccessor::ExportAsset			( const Path & expAssetFilePath, const Path & internalPath ) const
{
    auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMReadWrite );

    ExportAsset( *expFile.StreamBuf(), internalPath );

	expFile.Close();
}

// ********************************
//
void				AnimationAssetAccessor::ExportAsset			( std::ostream & out, const Path &  internalPath) const
{
    auto absPath = m_rootPath / internalPath;

    if( Path::Exists( absPath ) && Path::IsDir( absPath ) )
    {
        PathVec frames;
        for( auto fe : m_fileExts )
        {
            frames = Path::List( absPath, false, fe );
            if( frames.size() > 0 )
            {
                break;
            }
        }

        out << internalPath << '\n';
        out << std::to_string( frames.size() ) << '\n';

        for( auto f : frames )
        {
            out << f << '\n';
            out << std::to_string( File::Size( f.Str() ) ) << '\n';

            auto frameFile = File::Open( f.Str(), File::OpenMode::FOMReadOnly );

            frameFile.Read( out );

            frameFile.Close();
        }
    }
}

// ********************************
//
void			 	AnimationAssetAccessor::ExportAll			( std::ostream & out ) const
{
	for( auto p : ListAllUnique( m_rootPath ) )
	{
		ExportAsset( out, p );
	}
}

// ********************************
//
void				AnimationAssetAccessor::ExportAll			( const Path & expAssetFilePath ) const
{
    auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMReadWrite );

	ExportAll( *expFile.StreamBuf() );

	expFile.Close();
}

// ********************************
//
PathVec				AnimationAssetAccessor::ListAll				( const Path & path ) const
{
    auto pathList = Path::List( m_rootPath / path, true );

    PathVec ret; 

    for( auto p : pathList )
    {
        if( Path::IsDir( p ) )
        {
            auto ext = PathContainsAnimation( p );
            if( !ext.empty() )
            {
                ret.push_back( p );
            }
        }
    }
    
    return ret;
}


// ********************************
//
PathVec				AnimationAssetAccessor::ListAllUnique		( const Path & path ) const
{
	auto l = ListAll( path );

	std::set< Path  > unique;

	for( auto p : l )
	{
		unique.insert( p );
	}

	return PathVec( unique.begin(), unique.end() );	
}

// ********************************
//
std::string         AnimationAssetAccessor::PathContainsAnimation( const Path & path ) const
{
    for( auto fe : m_fileExts )
    {
        auto l = Path::List( path, false, fe );

        if( l.size() > 0 )
        {
            return fe;
        }
    }

    return "";
}

// ********************************
//
void				AnimationAssetAccessor::CreateDir() const
{
	if( !Dir::Exists( m_rootPath.Str() ) )
	{
		Dir::CreateDir( m_rootPath.Str(), true );
	}
}

} // bv