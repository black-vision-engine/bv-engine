#pragma once

#include "AssetAccessor.h"
#include "Assets/FwdDecls.h"


namespace bv {

class AudioAssetAccessor;
DEFINE_CONST_PTR_TYPE( AudioAssetAccessor )


class AudioAssetAccessor : public AssetAccessor
{
private:
    
    Path						m_rootPath;
	StringVector				m_fileExts;

public:

    static AudioAssetAccessorConstPtr Create        ( const Path & rootPath, const StringVector & fileExts );

                                ~AudioAssetAccessor ();

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

    virtual UInt64              GetAssetSizeInBytes ( const Path & path ) const override;

    virtual PathVec	            ListAll				( const Path & path, bool recursive ) const override;

    virtual PathVec	            ListAllUnique		( const Path & path ) const override;

private:

    explicit                    AudioAssetAccessor  ( const Path & rootPath, const StringVector & fileExts );

};


}   // bv
