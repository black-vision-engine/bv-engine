#pragma once

#include "ProjectManager/AssetAccessor.h"
#include "Assets/FwdDecls.h"

namespace bv
{

class ProjectManager;

class TextureAssetAccessor : public AssetAccessor
{

public:

	explicit					TextureAssetAccessor	( const ProjectManager * projectManager, const Path & rootPath, const StringVector & fileExts );
								~TextureAssetAccessor	();

	virtual AssetDescConstPtr	GetAssetDesc		( const Path & path ) const override;

    virtual void			 	AddAsset			( const Path & internalPath, const AssetDescConstPtr & assetDesc ) override;

    virtual void			 	RemoveAsset			( const Path & internalPath ) override;

    virtual void			 	RenameAsset			( const Path & oldPath, const Path & newPath) override;

    virtual void			 	ImportAsset			( const Path & impAssetFile, const Path &  importToPath ) override;

    virtual void			 	ExportAsset			( const Path & expAssetFilePath, const Path &  internalPath) const override;

    virtual void			 	ExportAll			( const Path & expAssetFilePath ) const override;

    virtual PathVec	ListAll				( const Path & path ) const override;

    virtual PathVec	ListAllUnique		( const Path & path ) const override;


private:

	void						CreateDir			() const;

	const ProjectManager *		m_projectManager;
	Path				m_rootPath;
	StringVector				m_fileExts;
};


} // bv