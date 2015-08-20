#pragma once

#include "NormalizedPath.h"

namespace bv
{

class AssetDesc;

class AssetAccessor
{
	virtual const AssetDesc *			GetLoadableAssetDesc( const NormalizedPath & path ) const  = 0 ;

    virtual void						AddAsset			( const NormalizedPath & internalPath, const AssetDesc & assetDesc ) = 0;

    virtual void						RemoveAsset			( const NormalizedPath & internalPath ) = 0;

    virtual void						RenameAsset			( const NormalizedPath & oldPath, const NormalizedPath & newPath) = 0;

    virtual void						ImportAsset			( const NormalizedPath & impAssetFile, const NormalizedPath &  importToPath ) = 0;

    virtual void						ExportAsset			( const NormalizedPath & expAssetFilePath, const NormalizedPath &  internalPath) const = 0;

    virtual void						ExportAll			( const NormalizedPath & expAssetFilePath ) const = 0;

    virtual NormalizedPathVec			ListAll				() const = 0;

    virtual NormalizedPathVec			ListAllUnique		( const NormalizedPath & path) const = 0;
};

} // bv