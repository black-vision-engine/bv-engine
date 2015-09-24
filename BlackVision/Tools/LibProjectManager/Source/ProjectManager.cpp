#include "ProjectManager.h"
#include "Impl/ProjectManagerImpl.h"

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
PathVec			ProjectManager::ListCategoriesNames	() const
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
void						ProjectManager::AddScene			( const model::BasicNode & sceneRootNode, const Path & projectName, const Path & outPath )
{
	m_impl->AddScene( sceneRootNode, projectName, outPath );
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
void						ProjectManager::ImportProjectFromFile( const Path & expFilePath, const Path & importToPath )
{
	m_impl->ImportProjectFromFile( expFilePath, importToPath );
}

// ********************************
//
AssetDescConstPtr			ProjectManager::GetAssetDesc		( const Path & projectName, const std::string & categoryName, const Path & pathInProject ) const
{
	return m_impl->GetAssetDesc( projectName, categoryName, pathInProject );
}

// ********************************
//
SceneDesc *					ProjectManager::GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const
{
	return m_impl->GetSceneDesc( projectName, pathInProject );
}

} // bv

