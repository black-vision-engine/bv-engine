#include "ProjectManager.h"
#include "Impl/ProjectManagerImpl.h"
#include "ConfigManager.h"

namespace bv
{
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
ProjectManager *			ProjectManager::GetInstance		( const Path & rootPath )
{
	auto it = g_pms.find( rootPath.Str() );

	if( it != g_pms.end() )
	{
		return it->second;
	}
	else
	{
		auto npm = new ProjectManager( rootPath );
		g_pms[ rootPath.Str() ] = npm;
		return npm;
	}
}

// ********************************
//
ProjectManager *	        ProjectManager::GetInstance		()
{
    auto pmRootFolder = ConfigManager::GetString( "PMFolder" );
    static auto instance = ProjectManager( pmRootFolder );
    return &instance;
}

// ********************************
//
PathVec			ProjectManager::ListProjectsNames	() const
{
	return m_impl->ListProjectsNames();
}

// ********************************
//
PathVec			ProjectManager::ListScenesNames		( const Path & projectName ) const
{
	return m_impl->ListScenesNames( projectName );
}

// ********************************
//
StringVector	ProjectManager::ListCategoriesNames	() const
{
	return m_impl->ListCategoriesNames();
}

// ********************************
//
PathVec			ProjectManager::ListAssetsPaths		( const Path & projectName,  const std::string & categoryName ) const
{
	return m_impl->ListAssetsPaths( projectName, categoryName );
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
void						ProjectManager::AddScene			( const model::BasicNodeConstPtr & sceneRootNode, const Path & projectName, const Path & outPath, model::TimelineManager * tm )
{
	m_impl->AddScene( sceneRootNode, projectName, outPath, tm );
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
void						ProjectManager::ImportSceneFromFile	( const Path & importToProjectName, const Path & importToPath, const Path & impSceneFilePath, model::TimelineManager * tm )
{
	m_impl->ImportSceneFromFile( importToProjectName, importToPath, impSceneFilePath, tm );
}

// ********************************
//
void						ProjectManager::ExportProjectToFile	( const Path & projectName, const Path & outputFilePath ) const
{
	m_impl->ExportProjectToFile( projectName, outputFilePath );
}

// ********************************
//
void						ProjectManager::ImportProjectFromFile( const Path & expFilePath, const Path & projectName, model::TimelineManager * tm )
{
	m_impl->ImportProjectFromFile( expFilePath, projectName, tm );
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
model::BasicNodeConstPtr     ProjectManager::LoadPreset          ( const Path & projectName, const Path & path ) const
{
    return m_impl->LoadPreset( projectName, path );
}

// ********************************
//
void                        ProjectManager::SavePreset          ( const model::BasicNodeConstPtr & node, const Path & projectName, const Path & path ) const
{
    m_impl->SavePreset( node, projectName, path );
}

// ********************************
//
PathVec                     ProjectManager::ListPresets         ( const Path & projectName, const Path & path ) const
{
    return m_impl->ListPresets( projectName, path );
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

// ********************************
//
Path                        ProjectManager::ToAbsPath           ( const Path & path ) const
{
    return m_impl->ToAbsPath( path );
}


} // bv

