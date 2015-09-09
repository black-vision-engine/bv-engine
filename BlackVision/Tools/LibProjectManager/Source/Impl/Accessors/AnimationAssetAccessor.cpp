#include "AnimationAssetAccessor.h"

#include "Assets/Texture/AnimationAssetDescriptor.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibBlackVision

namespace bv
{

// ********************************
//
AnimationAssetAccessor::AnimationAssetAccessor( const Path & rootPath, const StringVector & fileExts )
    : m_rootPath( rootPath )
    , m_fileExts( fileExts )
{}

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
    if( Path::Exists( path ) )
    {
        for( auto fe : m_fileExts )
        {
            if( Path::List( path, false, fe ).size() > 0 )
            {
                return AnimationAssetDesc::CreateFromDirFrames( path.Str(), fe );
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

        auto frames = typedDesc->GetFrames();

        for( auto f : frames )
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
    {impAssetFile;}
    {importToPath;}
}

// ********************************
//
void				AnimationAssetAccessor::ImportAsset			( std::istream & in, const Path &  importToPath ) const
{
    {in;}
    {importToPath;}
}

// ********************************
//
void				AnimationAssetAccessor::ExportAsset			( const Path & expAssetFilePath, const Path & internalPath ) const
{
    {expAssetFilePath;}
    {internalPath;}
}

// ********************************
//
void				AnimationAssetAccessor::ExportAsset			( std::ostream & out, const Path &  internalPath) const
{
    {out;}
    {internalPath;}
}

// ********************************
//
void			 	AnimationAssetAccessor::ExportAll			( std::ostream & out ) const
{
    {out;}
}

// ********************************
//
void				AnimationAssetAccessor::ExportAll			( const Path & expAssetFilePath ) const
{
    {expAssetFilePath;}
}

// ********************************
//
PathVec				AnimationAssetAccessor::ListAll				( const Path & path ) const
{
    {path;}
    return PathVec();
}


// ********************************
//
PathVec				AnimationAssetAccessor::ListAllUnique		( const Path & path ) const
{
    {path;}
    return PathVec();
}

} // bv