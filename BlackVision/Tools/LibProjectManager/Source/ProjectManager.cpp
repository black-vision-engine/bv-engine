#include "ProjectManager.h"
#include "Impl/ProjectManagerImpl.h"


namespace bv {

std::string                       ProjectManager::m_pmFolder = "DefaultPMDir";

std::shared_ptr< ProjectManager > ProjectManager::_instance = nullptr;

// ********************************
//
ProjectManager::ProjectManager	( const Path & rootPath )
    : m_impl( new ProjectManagerImpl( rootPath ) )
{}

// ********************************
//
ProjectManager::~ProjectManager	()
{
    delete m_impl;
}

// ********************************
//
namespace
{
    static std::map< std::string, ProjectManager * > g_pms;
}

// ********************************
//
ProjectManager *	        ProjectManager::GetInstance		()
{
    if( !_instance )
    {
        _instance = std::shared_ptr< ProjectManager >( new ProjectManager( m_pmFolder ) );
    }

    return _instance.get();
}

// ********************************
//
void	        ProjectManager::SetPMFolder		( const std::string & pmFolder )
{
    m_pmFolder = pmFolder;
}

// ********************************
// DO NOT USE, IF NOT TESTING MORE THAN ONE PM
void            ProjectManager::SetPMInstanceOnlyForTests( ProjectManager * inst )
{
    ProjectManager::_instance = std::shared_ptr< ProjectManager >( inst );
}


// ********************************
//
PathVec			ProjectManager::ListProjectsNames	() const
{
    return m_impl->ListProjectsNames();
}

// ********************************
//
PathVec			ProjectManager::ListScenesNames		( const Path & projectName, const Path & path, bool recursive ) const
{
    return m_impl->ListScenesNames( projectName, path, recursive );
}

// ********************************
//
StringVector	ProjectManager::ListCategoriesNames	() const
{
    return m_impl->ListCategoriesNames();
}

// ********************************
//
PathVec			ProjectManager::ListAssetsPaths		( const Path & projectName,  const std::string & categoryName, const Path & path, bool recursive ) const
{
    return m_impl->ListAssetsPaths( projectName, categoryName, path, recursive );
}

// ********************************
//
Path				ProjectManager::GetRootDir			() const
{
    return m_impl->GetRootDir();
}

// ********************************
//
void						ProjectManager::AddNewProject		( const Path & projectName )
{
    m_impl->AddNewProject( projectName );
}

// ********************************
//
ProjectConstPtr 			ProjectManager::GetProject			( const Path & projectName ) const
{
    return m_impl->GetProject( projectName );
}

// ********************************
//
void						ProjectManager::SetCurrentProject	( const Path & projectName )
{
    m_impl->SetCurrentProject( projectName );
}

// ********************************
//
void						ProjectManager::AddAsset			( const Path & projectName, const std::string & categoryName, const Path & path, const AssetDescConstPtr & assetDesc )
{
    m_impl->AddAsset( projectName, categoryName, path, assetDesc );
}

// ********************************
//
void						ProjectManager::CopyAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
    m_impl->CopyAsset( inProjectName, inCategoryName, inPath, outProjectName, outPath );
}

// ********************************
//
void						ProjectManager::RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path )
{
    m_impl->RemoveAsset( projectName, categoryName, path );
}

// ********************************
//
void						ProjectManager::MoveAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
    m_impl->MoveAsset( inProjectName, inCategoryName, inPath, outProjectName, outPath );
}

// ********************************
//
void						ProjectManager::RemoveUnusedAssets	( const Path & projectName, const std::string & categoryName )
{
    m_impl->RemoveUnusedAssets( projectName, categoryName );
}

// ********************************
//
void						ProjectManager::RemoveUnusedAssets	( const Path & projectName )
{
    m_impl->RemoveUnusedAssets( projectName );
}

// ********************************
//
void						ProjectManager::RemoveUnusedAssets	()
{
    m_impl->RemoveUnusedAssets();
}

// ********************************
//
void						ProjectManager::AddScene			( const model::SceneModelPtr & scene, const Path & projectName, const Path & outPath )
{
    m_impl->AddScene( scene, projectName, outPath );
}

// ********************************
//
void						ProjectManager::CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
    m_impl->CopyScene( inProjectName, inPath, outProjectName, outPath );
}

// ********************************
//
void						ProjectManager::RemoveScene			( const Path & projectName, const Path & path )
{
    m_impl->RemoveScene( projectName, path );
}

// ********************************
//
void						ProjectManager::MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
    m_impl->MoveScene( inProjectName, inPath, outProjectName, outPath );
}

// ********************************
//
LoadSceneResult             ProjectManager::LoadScene           ( const Path & projectName, const Path & path ) const
{
    return m_impl->LoadScene( projectName, path );
}

// ***********************
//
ThumbnailConstPtr           ProjectManager::GetSceneThumbnail   ( const Path & projectName, const Path & path ) const
{
    return m_impl->GetSceneThumbnail( projectName, path );
}

// ********************************
//
void						ProjectManager::RegisterCategory( const AssetCategoryConstPtr & category)
{
    m_impl->RegisterCategory( category );
}

// ********************************
//
void						ProjectManager::ExportAssetToFile	( const Path & projectName, const std::string & categoryName, const Path & assetPath, const Path & outputFile )
{
    m_impl->ExportAssetToFile( projectName, categoryName, assetPath, outputFile );
}

// ********************************
//
void						ProjectManager::ImportAssetFromFile	( const Path & importToProjectName, const std::string & importToCategoryName, const Path & importToPath, const Path & importAssetFilePath )
{
    m_impl->ImportAssetFromFile( importToProjectName, importToCategoryName, importToPath, importAssetFilePath );
}
    
// ********************************
//
void						ProjectManager::ExportSceneToFile	( const Path & projectName, const Path & scenePath, const Path & outputFile ) const
{
    m_impl->ExportSceneToFile( projectName, scenePath, outputFile );
}

// ********************************
//
void						ProjectManager::ImportSceneFromFile	( const Path & importToProjectName, const Path & importToPath, const Path & impSceneFilePath )
{
    m_impl->ImportSceneFromFile( importToProjectName, importToPath, impSceneFilePath );
}

// ********************************
//
void						ProjectManager::ExportProjectToFile	( const Path & projectName, const Path & outputFilePath ) const
{
    m_impl->ExportProjectToFile( projectName, outputFilePath );
}

// ********************************
//
void						ProjectManager::ImportProjectFromFile( const Path & expFilePath, const Path & projectName )
{
    m_impl->ImportProjectFromFile( expFilePath, projectName );
}

// ********************************
//
AssetDescConstPtr			ProjectManager::GetAssetDesc		( const Path & projectName, const std::string & categoryName, const Path & pathInProject ) const
{
    return m_impl->GetAssetDesc( projectName, categoryName, pathInProject );
}

// ********************************
//
SceneDescriptor				ProjectManager::GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const
{
    return m_impl->GetSceneDesc( projectName, pathInProject );
}

// ********************************
//
SceneDescriptor			    ProjectManager::GetSceneDesc		( const Path & path ) const
{
    return m_impl->GetSceneDesc( path );
}

// ********************************
//
model::BasicNodePtr        ProjectManager::LoadPreset          ( const Path & projectName, const Path & path, const model::OffsetTimeEvaluatorPtr & timeline ) const
{
    return m_impl->LoadPreset( projectName, path, timeline );
}

// ********************************
//
void                        ProjectManager::SavePreset          ( const model::BasicNodePtr & node, const Path & projectName, const Path & path ) const
{
    m_impl->SavePreset( node, projectName, path );
}

// ********************************
//
PathVec                     ProjectManager::ListPresets         ( const Path & projectName, const Path & path, bool recursive ) const
{
    return m_impl->ListPresets( projectName, path, recursive );
}

// ********************************
//
PathVec                     ProjectManager::ListPresets         ( const Path & projectName ) const
{
    return m_impl->ListPresets( projectName );
}

// ********************************
//
PathVec                     ProjectManager::ListPresets         () const
{
    return m_impl->ListPresets();
}

// ***********************
//
ThumbnailConstPtr           ProjectManager::GetPresetThumbnail  ( const Path & projectName, const Path & path ) const
{
    return m_impl->GetPresetThumbnail( projectName, path );
}


// ********************************
//
PathVec                     ProjectManager::ListAssetsDirs      ( const std::string & categoryName, const Path & path ) const
{
    return m_impl->ListAssetsDirs( categoryName, path );
}

// ********************************
//
PathVec                     ProjectManager::ListScenesDirs      ( const Path & path ) const
{
    return m_impl->ListScenesDirs( path );
}

// ********************************
//
PathVec                     ProjectManager::ListPresetsDirs     ( const Path & path ) const
{
    return m_impl->ListPresetsDirs( path );
}

// ********************************
//
bool                        ProjectManager::CreateAssetDir      ( const std::string & categoryName, const Path & path, bool recursive ) const
{
    return m_impl->CreateAssetDir( categoryName, path, recursive );
}

// ********************************
//
bool                        ProjectManager::CreateSceneDir      ( const Path & path, bool recursive ) const
{
    return m_impl->CreateSceneDir( path, recursive );
}

// ********************************
//
bool                        ProjectManager::CreatePresetDir      ( const Path & path, bool recursive ) const
{
    return m_impl->CreatePresetDir( path, recursive );
}

// ********************************
//
bool                        ProjectManager::RemoveAssetDir      ( const std::string & categoryName, const Path & path ) const
{
    return m_impl->RemoveAssetDir( categoryName, path );
}

// ********************************
//
bool                        ProjectManager::RemoveSceneDir      ( const Path & path ) const
{
    return m_impl->RemoveSceneDir( path );
}

// ********************************
//
bool                        ProjectManager::RemovePresetDir     ( const Path & path ) const
{
    return m_impl->RemovePresetDir( path );
}

// ********************************
//
bool                        ProjectManager::RenameAssetDir      ( const std::string & categoryName, const Path & path, const std::string &newName ) const
{
    return m_impl->RenameAssetDir( categoryName, path, newName );
}

// ********************************
//
bool                        ProjectManager::RenameSceneDir      ( const Path & path, const std::string &newName ) const
{
    return m_impl->RenameSceneDir( path, newName );
}

// ********************************
//
bool                        ProjectManager::CopyPresetDir       ( const Path & path, const std::string &newName ) const
{
    return m_impl->CopyPresetDir( path, newName );
}

// ********************************
//
bool                        ProjectManager::CopyAssetDir        ( const std::string & categoryName, const Path & path, const std::string &newName ) const
{
    return m_impl->CopyAssetDir( categoryName, path, newName );
}

// ********************************
//
bool                        ProjectManager::CopySceneDir        ( const Path & path, const std::string &newName ) const
{
    return m_impl->CopySceneDir( path, newName );
}

// ********************************
//
bool                        ProjectManager::RenamePresetDir      ( const Path & path, const std::string &newName ) const
{
    return m_impl->RenamePresetDir( path, newName );
}

// ********************************
//
Path                        ProjectManager::ToAbsPath           ( const Path & path ) const
{
    return m_impl->ToAbsPath( path );
}

// ********************************
//
UInt64                      ProjectManager::GetAssetSize        ( const std::string & categoryName, const Path & path ) const
{
    return m_impl->GetAssetSize( categoryName, path );
}

// ********************************
//
UInt32                      ProjectManager::GetAssetCount       ( const std::string & categoryName, const Path & path ) const
{
    return m_impl->GetAssetCount( categoryName, path );
}

// ********************************
//
UInt32                      ProjectManager::GetScenesCount      ( const Path & path ) const
{
    return m_impl->GetScenesCount( path );
}

// ********************************
//
UInt32                      ProjectManager::GetPresetsCount     ( const Path & path ) const
{
    return m_impl->GetPresetsCount( path );
}

} // bv

