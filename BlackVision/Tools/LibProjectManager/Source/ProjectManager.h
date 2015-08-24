#pragma once

#include "Assets/FwdDecls.h"

#include "System/Path.h"

namespace bv
{

class Project;
class Category;
class SceneDesc;
class ProjectManagerImpl;

namespace model 
{
	class BasicNode;
} // model

class ProjectManager
{
public:

	// listing
	PathVec					ListProjectsNames	() const;
	PathVec					ListScenesNames		( const Path & projectName = Path("") ) const;
	PathVec					ListCategoriesNames	() const;
	PathVec					ListAssetsPaths		( const Path & projectName,  const std::string & categoryName = "" ) const;

	PathVec					ListAllUsedAssets	() const;
	PathVec					ListAllUnusedAssets	( const Path & projectName, const std::string & categoryName ) const;

	Path					GetRootDir			() const;

	// projects
	void					AddNewProject		( const Path & projectName );
	const Project *			GetProject			( const Path & projectName ) const;
	void					SetCurrentProject	( const Path & projectName );

	// assets
	void					AddAsset			( const Path & projectName, const std::string & categoryName, const Path & path, const AssetDescConstPtr & assetDesc );

	// 
	void					CopyAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path );
	void					MoveAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveUnusedAssets	( const Path & projectName, const std::string & categoryName );
	void					RemoveUnusedAssets	( const Path & projectName );

	void					AddScene			( const model::BasicNode & sceneRootNode, const Path & projectName, const Path & outPath );
	void					CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveScene			( const Path & projectName, const Path & path );
	void					MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );

	// categories
	void					RegisterGlobalCategory( const Category & category);


	// *********************************
	// exporting importing

	// assets
	void					ExportAssetToFile	( const Path & projectName, const std::string & categoryName, const Path & assetPath, const Path & outputFile );
	void					ImportAssetFromFile	( const Path & importToProjectName, const std::string & importToCategoryName, const Path & importToPath, const Path & importAssetFilePath );
	
	// scenes
	void					ExportSceneToFile	( const Path & projectName, const Path & scenePath, const Path & outputFile ) const;
	void					ImportSceneFromFile	( const Path & importToProjectName, const Path & importToPath, const Path & impSceneFilePath );

	// projects
	void					ExportProjectToFile	( const Path & projectName, const Path &  outputFilePath ) const;
	void					ImportProjectFromFile( const Path & expFilePath, const Path & importToPath );

	// *********************************
	// getting scenes and assets descriptors
	
	//AssetDesc				getAssetDescLoc		( loc );
	AssetDescConstPtr		GetAssetDesc		( const Path & projectName, const std::string & categoryName, const Path & pathInProject ) const;

	//SceneDesc				GetSceneDescLoc		( loc )
	SceneDesc *				GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const;

	static ProjectManager *	GetInstance		();

private:
	ProjectManagerImpl * m_impl;

	ProjectManager	();
	~ProjectManager	();
};

} // bv