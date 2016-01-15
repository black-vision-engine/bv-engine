#pragma once

#include "AssetCategory.h"
#include "Project.h"
#include "SceneDescriptor.h"

#include "Engine/Models/BVProject.h"

#include "Assets/FwdDecls.h"

#include "System/Path.h"

namespace bv
{

void ChangeProjectManagerInstanceTo( const std::string & );

class Project;
class AssetCategory;
class ProjectManagerImpl;

namespace model 
{
	class BasicNode;
    class TimelineManager;
} // model

class ProjectManager
{
public:

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
	void					RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path );
	void					MoveAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveUnusedAssets	( const Path & projectName, const std::string & categoryName );
    void					RemoveUnusedAssets	( const Path & projectName );
	void					RemoveUnusedAssets	();

	void					AddScene			( const model::SceneModelPtr & scene, const Path & projectName, const Path & outPath );
	void					CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveScene			( const Path & projectName, const Path & path );
	void					MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );
    model::SceneModelPtr    LoadScene           ( const Path & projectName, const Path & path ) const;

	// categories
	void					RegisterCategory	( const AssetCategoryConstPtr & category);


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
	void					ImportProjectFromFile( const Path & expFilePath, const Path & projectName );

	// *********************************
	// getting scenes and assets descriptors
	
	//AssetDesc				getAssetDescLoc		( loc );
	AssetDescConstPtr		GetAssetDesc		( const Path & projectName, const std::string & categoryName, const Path & pathInProject ) const;

	//SceneDesc				GetSceneDescLoc		( loc )
	SceneDescriptor			GetSceneDesc		( const Path & path ) const;
	SceneDescriptor			GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const;

	// *********************************
	// loading, saving presets
    model::SceneModelPtr    LoadPreset          ( const Path & projectName, const Path & path ) const;
    void                    SavePreset          ( const model::SceneModelPtr & node, const Path & projectName, const Path & path ) const;
    PathVec                 ListPresets         ( const Path & projectName, const Path & path ) const;
    PathVec                 ListPresets         ( const Path & projectName ) const;
    PathVec                 ListPresets         () const;

    // *********************************
	// handling directories
    PathVec                 ListAssetsDirs      ( const std::string & categoryName, const Path & path ) const;
    PathVec                 ListScenesDirs      ( const Path & path ) const;
    bool                    CreateAssetDir      ( const std::string & categoryName, const Path & path ) const;
    bool                    CreateSceneDir      ( const Path & path ) const;
    bool                    RemoveAssetDir      ( const std::string & categoryName, const Path & path ) const;
    bool                    RemoveSceneDir      ( const Path & path ) const;

    Path                    ToAbsPath           ( const Path & path ) const;

    static ProjectManager *	GetInstance			(  );

	~ProjectManager	();

private:
	ProjectManagerImpl * m_impl;

	ProjectManager	( const Path & rootPath );

    static std::shared_ptr< ProjectManager > _instance;

    static void             SetPMInstanceOnlyForTests( ProjectManager * inst );
    friend void             bv::ChangeProjectManagerInstanceTo( const std::string & );
};

} // bv