#include "FontAssetAccessor.h"
#include "Assets/Font/2D/FontAssetDescriptor.h"
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
FontAssetAccessorConstPtr FontAssetAccessor::Create( const Path & rootPath, const StringVector & fileExts )
{
    return FontAssetAccessorConstPtr( new FontAssetAccessor( rootPath, fileExts ) );
}

// ********************************
//
FontAssetAccessor::FontAssetAccessor				( const Path & rootPath, const StringVector & fileExts )
    : m_rootPath( rootPath )
    , m_fileExts( fileExts )
{
    CreateDir();
}

// ********************************
//
FontAssetAccessor::~FontAssetAccessor				()
{}

// ********************************
//
AssetDescConstPtr	FontAssetAccessor::GetAssetDesc	( const Path & path ) const
{
    auto p = m_rootPath / path;

    if( Path::Exists( p ) )
    {
        return FontAssetDesc::Create( ( Path( "fonts" ) / path ).Str(), 0, 0, 0, 0, true );
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "Asset '" << p.Str() << "' doesn't exist.";
        return nullptr;
    }
}

// ********************************
//
void			 	FontAssetAccessor::AddAsset		( const Path & internalPath, const AssetDescConstPtr & assetDesc ) const
{
    auto uid = assetDesc->GetUID();

    if( uid == FontAssetDesc::UID() )
    {
        auto typedDesc = QueryTypedDesc< FontAssetDescConstPtr >( assetDesc );

        auto path = typedDesc->GetFontFileName();

        Path::Copy( path, m_rootPath / internalPath );
    }
    else
    {
        assert( !"Wrong asset descriptor type" );
    }
}

// ********************************
//
void			 	FontAssetAccessor::RemoveAsset	( const Path & internalPath ) const
{
    Path::Remove( m_rootPath / internalPath );
}

// ********************************
//
void			 	FontAssetAccessor::RenameAsset	( const Path & oldPath, const Path & newPath ) const
{
    Path::Rename( m_rootPath / oldPath, m_rootPath / newPath );
}

// ********************************
//
void			 	FontAssetAccessor::ExportAsset	( const Path & expAssetFilePath, const Path & internalPath) const
{
    auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMWriteAppend );

    auto out = expFile.StreamBuf();

    ExportAsset( *out, internalPath );

    expFile.Close();
}

// ********************************
//
void			 	FontAssetAccessor::ExportAsset	( std::ostream & out, const Path & internalPath) const
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
void			 	FontAssetAccessor::ImportAsset	( const Path & impAssetFile, const Path & importToPath ) const
{
    auto impAsset = File::Open( impAssetFile.Str(), File::OpenMode::FOMReadOnly );

    auto in = impAsset.StreamBuf();

    ImportAsset( *in, importToPath );
}

// ********************************
//
void				FontAssetAccessor::ImportAsset	( std::istream & in, const Path &  importToPath ) const
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
void			 	FontAssetAccessor::ExportAll		( std::ostream & out ) const
{
    for( auto p : ListAllUnique( m_rootPath ) )
    {
        ExportAsset( out, p );
    }
}

// ********************************
//
void			 	FontAssetAccessor::ExportAll		( const Path & expAssetFilePath ) const
{
    auto expFile = File::Open( expAssetFilePath.Str(), File::OpenMode::FOMWriteAppend );

    auto out = expFile.StreamBuf();

    ExportAll( *out );

    expFile.Close();
}

// ********************************
//
PathVec	FontAssetAccessor::ListAll		( const Path & path, bool recursive ) const
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
PathVec	FontAssetAccessor::ListAllUnique	( const Path & path ) const
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
void				FontAssetAccessor::CreateDir		() const
{
    if( !Dir::Exists( m_rootPath.Str() ) )
    {
        Dir::CreateDir( m_rootPath.Str(), true );
    }
}

// ********************************
//
UInt64              FontAssetAccessor::GetAssetSizeInBytes ( const Path & path ) const
{
    return File::GetSize( ( m_rootPath / path ).Str() );
}

} // bv