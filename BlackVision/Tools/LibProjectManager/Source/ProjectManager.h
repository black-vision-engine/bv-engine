#pragma once

#include "AssetCategory.h"
#include "Project.h"
#include "SceneDescriptor.h"

#include "Engine/Editors/BVProject.h"

#include "Assets/FwdDecls.h"

#include "System/Path.h"


namespace bv
{

void ChangeProjectManagerInstanceTo( const std::string & );

class Project;
class AssetCategory;
class ProjectManagerImpl;

class Thumbnail;
DEFINE_CONST_PTR_TYPE( Thumbnail )

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
    void					RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path );
    void					MoveAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath );
    void					RemoveUnusedAssets	( const Path & projectName, const std::string & categoryName );
    void					RemoveUnusedAssets	( const Path & projectName );
    void					RemoveUnusedAssets	();

    void					            AddScene			( const model::SceneModelPtr & scene, const Path & projectName, const Path & outPath );
    void					            CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );
    void					            RemoveScene			( const Path & projectName, const Path & path );
    void					            MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath );
    Expected< model::SceneModelPtr >    LoadScene           ( const Path & projectName, const Path & path ) const;
    ThumbnailConstPtr                   GetSceneThumbnail   ( const Path & projectName, const Path & path ) const;

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
    bool                    CreatePresetDir     ( const Path & path, bool recursive) const;

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

    Path                    ToAbsPath           ( const Path & path ) const;

    static ProjectManager *	GetInstance			();

    static void             SetPMFolder         ( const std::string & pmFolder );

    ~ProjectManager	();

private:

    ProjectManagerImpl * m_impl;

    ProjectManager	( const Path & rootPath );

    static std::shared_ptr< ProjectManager > _instance;

    static std::string      m_pmFolder;

    static void             SetPMInstanceOnlyForTests( ProjectManager * inst );
    friend void             bv::ChangeProjectManagerInstanceTo( const std::string & );

};

} // bv