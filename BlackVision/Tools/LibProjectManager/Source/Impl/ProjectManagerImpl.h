#pragma once

#include "CoreDEF.h"

#include "ProjectManager.h"
#include "Project.h"

#include  "Accessors/SceneAccessor.h"

#include <map>

namespace bv
{

class ProjectManagerImpl
{
private:

	// listing
	PathVec					ListProjectsNames	() const;
	PathVec					ListScenesNames		( const Path & projectName = Path("") ) const;
	StringVector			ListCategoriesNames	() const;
	PathVec					ListAssetsPaths		( const Path & projectName,  const std::string & categoryName = "" ) const;

	PathVec					ListAllUsedAssets	() const;
	PathVec					ListAllUnusedAssets	( const Path & projectName, const std::string & categoryName ) const;

	Path					GetRootDir			() const;

	// projects
	void					AddNewProject		( const Path & projectName );
	ProjectConstPtr			GetProject			( const Path & projectName ) const;
	void					SetCurrentProject	( const Path & projectName );

	// assets
	void					AddAsset			( const Path & projectName, const std::string & categoryName, const Path & path, const AssetDescConstPtr & assetDesc );

	// 
	void					CopyAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path ) const;
	void					MoveAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveUnusedAssets	( const Path & projectName, const std::string & categoryName ) const;
	void					RemoveUnusedAssets	( const Path & projectName ) const;
    void				    RemoveUnusedAssets	() const;

	void					AddScene			( const model::BasicNodeConstPtr & sceneRootNode, const Path & projectName, const Path & outPath );
	void					CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveScene			( const Path & projectName, const Path & path );
	void					MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );

	// categories
	void					RegisterCategory	( const AssetCategoryConstPtr & category);


	// *********************************
	// exporting/importing

	// assets
	void					ExportAssetToFile	( const Path & projectName, const std::string & categoryName, const Path & assetPath, const Path & outputFile );
	void					ImportAssetFromFile	( const Path & importToProjectName, const std::string & importToCategoryName, const Path & importToPath, const Path & importAssetFilePath );
	
	// scenes
	void					ExportSceneToFile	( const Path & projectName, const Path & scenePath, const Path & outputFile ) const;
	void					ImportSceneFromFile	( const Path & importToProjectName, const Path & importToPath, const Path & impSceneFilePath );

	// projects
	void					ExportProjectToFile	( const Path & projectName, const Path &  outputFilePath ) const;
	void					ImportProjectFromFile( const Path & expFilePath, const Path & projectName );

	// *********************************
	// getting scenes and assets descriptors
	
	//AssetDesc				getAssetDescLoc		( loc );
	AssetDescConstPtr		GetAssetDesc		( const Path & projectName, const std::string & categoryName, const Path & pathInProject ) const;

	//SceneDesc				GetSceneDescLoc		( loc )
	SceneDescriptor			GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const;

	void					InitializeProjects	();
	void					InitializeScenes	();

	Path					TranslateToPathCategory( const Path & projectName, const Path & path ) const;
	Path					TranslateToPathInPMRootFolder( const Path & projectName, const std::string & categoryName, const Path & path ) const;

	std::string				GetCategoryName		( const Path & path ) const;
	Path					GetProjectName		( const Path & path ) const;

	explicit ProjectManagerImpl( const Path & rootPath, model::TimelineManager * tm );
			~ProjectManagerImpl();


	typedef std::map< std::string, AssetCategoryConstPtr >	CategoryMap;
	typedef std::map< std::string, ProjectConstPtr >		ProjectMap;

	ProjectConstPtr			m_currentProject;

	CategoryMap				m_categories;
	ProjectMap				m_projects;

	SceneAccessorConstPtr	m_sceneAccessor;

	Path					m_rootPath;
	Path					m_projectsPath;
	Path					m_scenesPath;

    model::TimelineManager * m_timelineManager; 

	friend ProjectManager;

	struct Location
	{
		std::string		categoryName;
		Path			projectName;
		Path			path;
	};

	Location				Path2Location		( const Path & path ) const;
	Path					Location2Path		( const Location & loc ) const;
};


} // bv