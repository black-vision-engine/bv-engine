#include "AnimationAssetAccessor.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"

#include "IO/FileIO.h"
#include "IO/DirIO.h"

#include "LibImage.h"

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
    auto p = m_rootPath / path;

    if( Path::Exists( p ) )
    {
        for( auto fe : m_fileExts )
        {
            auto pathList = Path::List( p, false, fe );

            if( !pathList.empty() )
            {
                auto numFrames = pathList.size();

                auto props = image::GetImageProps( pathList[ 0 ].Str() );

                return AnimationAssetDesc::Create( ( Path( "sequences" ) / path ).Str(), numFrames, props.width, props.height, fe );
            }
        }
    }

    LOG_MESSAGE( SeverityLevel::warning ) << "Asset '" << p.Str() << "' doesn't exist.";
    
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

        auto files = Path::List( typedDesc->GetPath(), false, typedDesc->GetFilter() );

        TextureAssetDescVec framesDesc;

        if ( files.size() > 0 )
        {
            for( auto f : files )
            {
                framesDesc.push_back( TextureAssetDesc::Create( f.Str(), true ) );
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
PathVec				AnimationAssetAccessor::ListAll				( const Path & path, bool recursive ) const
{
    PathVec ret;

    if( PathContainsAnimation( m_rootPath / path ) )
    {
        ret.push_back( path );
        return ret;
    }

    auto pathList = Path::List( m_rootPath / path, recursive );


    for( auto p : pathList )
    {
        if( Path::IsDir( p ) )
        {
            if( PathContainsAnimation( p ) )
            {
                ret.push_back( p );
            }
        }
    }

    for( auto & p : ret )
    {
        p = Path::RelativePath( p, m_rootPath );
    }
    
    return ret;
}


// ********************************
//
PathVec				AnimationAssetAccessor::ListAllUnique		( const Path & path ) const
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
bool                AnimationAssetAccessor::PathContainsAnimation( const Path & path ) const
{
    for( auto fe : m_fileExts )
    {
        auto l = Path::List( path, false, fe );

        if( l.size() > 0 )
        {
            return true;
        }
    }

    return false;
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

// ********************************
//
UInt64              AnimationAssetAccessor::GetAssetSizeInBytes ( const Path & path ) const
{
    return Dir::GetSize( ( m_rootPath / path ).Str() );
}

} // bv
