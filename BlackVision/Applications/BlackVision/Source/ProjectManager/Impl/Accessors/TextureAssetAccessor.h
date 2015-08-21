#pragma once

#include "ProjectManager/AssetAccessor.h"

#include "CoreDEF.h"

namespace bv
{

class ProjectManager;

class TextureAssetAccessor : public AssetAccessor
{

public:

	explicit					TextureAssetAccessor	( const ProjectManager * projectManager, const NormalizedPath & rootPath, const StringVector & fileExts );
								~TextureAssetAccessor	();

	virtual const AssetDesc *	GetAssetDesc		( const NormalizedPath & path ) const override;

    virtual void			 	AddAsset			( const NormalizedPath & internalPath, const AssetDesc & assetDesc ) override;

    virtual void			 	RemoveAsset			( const NormalizedPath & internalPath ) override;

    virtual void			 	RenameAsset			( const NormalizedPath & oldPath, const NormalizedPath & newPath) override;

    virtual void			 	ImportAsset			( const NormalizedPath & impAssetFile, const NormalizedPath &  importToPath ) override;

    virtual void			 	ExportAsset			( const NormalizedPath & expAssetFilePath, const NormalizedPath &  internalPath) const override;

    virtual void			 	ExportAll			( const NormalizedPath & expAssetFilePath ) const override;

    virtual NormalizedPathVec	ListAll				( const NormalizedPath & path ) const override;

    virtual NormalizedPathVec	ListAllUnique		( const NormalizedPath & path ) const override;


private:
	const ProjectManager *		m_projectManager;
	NormalizedPath				m_rootPath;
	StringVector				m_fileExts;
};


} // bv