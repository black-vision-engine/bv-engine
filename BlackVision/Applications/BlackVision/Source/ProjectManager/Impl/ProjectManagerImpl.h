#pragma once

#include "CoreDEF.h"

#include "ProjectManager/ProjectManager.h"

namespace bv
{

class ProjectManagerImpl
{
private:

	// listing
	NormalizedPathVec				ListProjectsNames	() const;
	NormalizedPathVec				ListScenesNames		( const NormalizedPath & projectName = NormalizedPath("") ) const;
	NormalizedPathVec				ListCategoriesNames	() const;
	NormalizedPathVec				ListAssetsPaths		( const NormalizedPath & projectName,  const std::string & categoryName = "" ) const;

	NormalizedPathVec				ListAllUsedAssets	() const;
	NormalizedPathVec				ListAllUnusedAssets	( const NormalizedPath & projectName, const std::string & categoryName ) const;

	NormalizedPath					GetRootDir			() const;

	// projects
	void							AddNewProject		( const NormalizedPath & projectName );
	const Project *					GetProject			( const NormalizedPath & projectName ) const;
	void							SetCurrentProject	( const NormalizedPath & projectName );

	// assets
	void							AddAsset			( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & path, const AssetDesc & assetDesc );

	// 
	void							CopyAsset			( const NormalizedPath & inProjectName, const std::string & inCategoryName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath );
	void							RemoveAsset			( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & path );
	void							MoveAsset			( const NormalizedPath & inProjectName, const std::string & inCategoryName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath );
	void							RemoveUnusedAssets	( const NormalizedPath & projectName, const std::string & categoryName );
	void							RemoveUnusedAssets	( const NormalizedPath & projectName );

	void							AddScene			( const model::BasicNode & sceneRootNode, const NormalizedPath & projectName, const NormalizedPath & outPath );
	void							CopyScene			( const NormalizedPath & inProjectName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath );
	void							RemoveScene			( const NormalizedPath & projectName, const NormalizedPath & path );
	void							MoveScene			( const NormalizedPath & inProjectName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath );

	// categories
	void							RegisterGlobalCategory( const Category & category);


	// *********************************
	// exporting/importing

	// assets
	void							ExportAssetToFile	( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & assetPath, const NormalizedPath & outputFile );
	void							ImportAssetFromFile	( const NormalizedPath & importToProjectName, const std::string & importToCategoryName, const NormalizedPath & importToPath, const NormalizedPath & importAssetFilePath );
	
	// scenes
	void							ExportSceneToFile	( const NormalizedPath & projectName, const NormalizedPath & scenePath, const NormalizedPath & outputFile ) const;
	void							ImportSceneFromFile	( const NormalizedPath & importToProjectName, const NormalizedPath & importToPath, const NormalizedPath & impSceneFilePath );

	// projects
	void							ExportProjectToFile	( const NormalizedPath & projectName, const NormalizedPath &  outputFilePath ) const;
	void							ImportProjectFromFile( const NormalizedPath & expFilePath, const NormalizedPath & importToPath );

	// *********************************
	// getting scenes and assets descriptors
	
	//AssetDesc						getAssetDescLoc		( loc );
	AssetDesc *						GetAssetDesc		( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & pathInProject ) const;

	//SceneDesc						GetSceneDescLoc		( loc )
	SceneDesc *						GetSceneDesc		( const NormalizedPath & projectName, const NormalizedPath & pathInProject ) const;

	explicit ProjectManagerImpl();
			~ProjectManagerImpl();

	friend ProjectManager;
};


} // bv