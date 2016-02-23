#pragma once

#include "AssetAccessor.h"
#include "Assets/FwdDecls.h"

namespace bv
{

class VideoStreamAssetAccessor;
DEFINE_CONST_PTR_TYPE( VideoStreamAssetAccessor )




class VideoStreamAssetAccessor : public AssetAccessor
{
private:
    
    Path						m_rootPath;
	StringVector				m_fileExts;

public:

    static VideoStreamAssetAccessorConstPtr Create( const Path & rootPath, const StringVector & fileExts );

                                ~VideoStreamAssetAccessor();

	virtual AssetDescConstPtr	GetAssetDesc		( const Path & path ) const override;

    virtual void			 	AddAsset			( const Path & internalPath, const AssetDescConstPtr & assetDesc ) const override;

    virtual void			 	RemoveAsset			( const Path & internalPath ) const override;

    virtual void			 	RenameAsset			( const Path & oldPath, const Path & newPath) const override;

    virtual void			 	ImportAsset			( const Path & impAssetFile, const Path &  importToPath ) const override;
	virtual void				ImportAsset			( std::istream & in, const Path &  importToPath ) const override;

    virtual void			 	ExportAsset			( const Path & expAssetFilePath, const Path &  internalPath) const override;
	virtual void				ExportAsset			( std::ostream & out, const Path &  internalPath) const override;

	virtual void			 	ExportAll			( std::ostream & out ) const override;
    virtual void			 	ExportAll			( const Path & expAssetFilePath ) const override;

    virtual PathVec	ListAll				( const Path & path, bool recursive ) const override;

    virtual PathVec	ListAllUnique		( const Path & path ) const override;

private:
    explicit            VideoStreamAssetAccessor( const Path & rootPath, const StringVector & fileExts );
};


}   // bv