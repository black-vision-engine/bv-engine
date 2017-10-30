#pragma once

#include "CoreDEF.h"

#include "ProjectManager.h"
#include "Project.h"

#include  "Accessors/SceneAccessor.h"
#include  "Accessors/PresetAccessor.h"

#include <map>

namespace bv
{

class ProjectManagerImpl
{
private:

	// listing
	PathVec					ListProjectsNames	() const;
	PathVec					ListScenesNames		( const Path & projectName, const Path & path, bool recursive ) const;
	StringVector			ListCategoriesNames	() const;
	PathVec					ListAssetsPaths		( const Path & projectName,  const std::string & categoryName, const Path & path, bool recursive ) const;

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

	void					AddScene			( const model::SceneModelPtr & scene, const Path & projectName, const Path & outPath );
	void					CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );
	void					RemoveScene			( const Path & projectName, const Path & path );
	void					MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );
    LoadSceneResult         LoadScene           ( const Path & projectName, const Path & path ) const;
    ThumbnailConstPtr       GetSceneThumbnail   ( const Path & projectName, const Path & path ) const;

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
	SceneDescriptor			GetSceneDesc		( const Path & path ) const;
	SceneDescriptor			GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const;

    // *********************************
	// loading, saving presets
    model::BasicNodePtr     LoadPreset          ( const Path & projectName, const Path & path, const model::OffsetTimeEvaluatorPtr & timeline ) const;
    void                    SavePreset          ( const model::BasicNodePtr & node, const Path & projectName, const Path & path ) const;
    PathVec                 ListPresets         ( const Path & projectName, const Path & path, bool recursive ) const;
    PathVec                 ListPresets         ( const Path & projectName ) const;
    PathVec                 ListPresets         () const;

    ThumbnailConstPtr       GetPresetThumbnail  ( const Path & projectName, const Path & path ) const;

    // *********************************
    // handling directories
    PathVec                 ListAssetsDirs      ( const std::string & categoryName, const Path & path ) const;
    PathVec                 ListScenesDirs      ( const Path & path ) const;
    PathVec                 ListPresetsDirs     ( const Path & path ) const;

    bool                    CreateAssetDir      ( const std::string & categoryName, const Path & path, bool recursive ) const;
    bool                    CreateSceneDir      ( const Path & path, bool recursive ) const;
    bool                    CreatePresetDir     ( const Path & path, bool recursive ) const;

    bool                    RemoveAssetDir      ( const std::string & categoryName, const Path & path ) const;
    bool                    RemoveSceneDir      ( const Path & path ) const;
    bool                    RemovePresetDir     ( const Path & path ) const;

  	bool                    RenameAssetDir      ( const std::string & categoryName, const Path & path, const std::string &newName ) const;
  	bool                    RenameSceneDir      ( const Path & path, const std::string &newName ) const;
  	bool                    RenamePresetDir     ( const Path & path, const std::string &newName ) const;

    bool                    CopyAssetDir        ( const std::string & categoryName, const Path & path, const std::string &newName ) const;
    bool                    CopySceneDir        ( const Path & path, const std::string &newName ) const;
    bool                    CopyPresetDir       ( const Path & path, const std::string &newName ) const;

    // *********************************
    // categories statistics
    UInt64                  GetAssetSize        ( const std::string & categoryName, const Path & path ) const;
    UInt32                  GetAssetCount       ( const std::string & categoryName, const Path & path ) const;
    UInt32                  GetScenesCount      ( const Path & path ) const;
    UInt32                  GetPresetsCount     ( const Path & path ) const;

	void					InitializeProjects	();
	void					InitializeScenes	();
    void				    InitializePresets	();
    void				    InitializeAssets	();

	Path					TranslateToPathCategory( const Path & projectName, const Path & path ) const;
	Path					TranslateToPathInPMRootFolder( const Path & projectName, const std::string & categoryName, const Path & path ) const;

    Path                    ToAbsPath           ( const Path & path ) const;
    bool                    IsExternalPath      ( const Path & path ) const;

	explicit ProjectManagerImpl( const Path & rootPath );
			~ProjectManagerImpl();


	typedef std::map< std::string, AssetCategoryConstPtr >	CategoryMap;
	typedef std::map< std::string, ProjectConstPtr >		ProjectMap;

	ProjectConstPtr			m_currentProject;

	CategoryMap				m_categories;
	ProjectMap				m_projects;

	SceneAccessorConstPtr	m_sceneAccessor;
    PresetAccessorConstPtr  m_presetAccessor;

	Path					m_rootPath;
	Path					m_projectsPath;
	Path					m_scenesPath;
    Path					m_presetsPath;

	friend ProjectManager;

	struct Location
	{
		std::string		categoryName;
		Path			projectName;
		Path			path;

        bool            IsValid () const
        {
            return !categoryName.empty();
        }
	};

	Location				Path2Location		( const Path & path ) const;
	Path					Location2Path		( const Location & loc ) const;
    bool                    IsValidPMPath       ( const Path & path ) const;
    bool                    PathExistsInPM      ( const Path & path ) const;
    bool                    CreateDir           ( const std::string & categoryName, const Path & path, bool recursive ) const;
    bool                    RemoveDir           ( const std::string & categoryName, const Path & path ) const;
    bool                    RenameDir           ( const std::string & categoryName, const Path & path, const std::string & newName ) const;
    bool                    CopyDir             ( const std::string & categoryName, const Path & path, const std::string & newName ) const;
    PathVec                 ListDirs            ( const std::string & categoryName, const Path & path, const Path & relativeTo = "" ) const;
};


} // bv