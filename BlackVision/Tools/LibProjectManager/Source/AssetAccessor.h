#pragma once

#include "Assets/FwdDecls.h"

#include "System/Path.h"

#include <ostream>

namespace bv
{

class AssetAccessor
{
public:

	virtual AssetDescConstPtr	GetAssetDesc		( const Path & path ) const  = 0 ;

    virtual void				AddAsset			( const Path & internalPath, const AssetDescConstPtr & assetDesc ) const = 0;

    virtual void				RemoveAsset			( const Path & internalPath ) const = 0;

    virtual void				RenameAsset			( const Path & oldPath, const Path & newPath) const = 0;

    virtual void				ImportAsset			( const Path & impAssetFile, const Path &  importToPath ) const = 0;
	virtual void				ImportAsset			( std::istream & in, const Path &  importToPath ) const = 0;

    virtual void				ExportAsset			( const Path & expAssetFilePath, const Path &  internalPath) const = 0;
	virtual void				ExportAsset			( std::ostream & out, const Path &  internalPath) const = 0;

	virtual void			 	ExportAll			( std::ostream & out ) const = 0;
    virtual void				ExportAll			( const Path & expAssetFilePath ) const = 0;

    virtual UInt64              GetAssetSizeInBytes ( const Path & path ) const = 0;

    virtual PathVec				ListAll				( const Path & path, bool recursive ) const = 0;

    virtual PathVec				ListAllUnique		( const Path & path ) const = 0;

    static Path                 GetThumbnailPath    ( const Path & absPath );

	virtual						~AssetAccessor		() = 0 {};
};

DEFINE_CONST_PTR_TYPE( AssetAccessor )
DEFINE_PTR_TYPE( AssetAccessor )

} // bv