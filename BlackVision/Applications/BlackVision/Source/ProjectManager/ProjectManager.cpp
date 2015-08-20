#include "ProjectManager.h"
#include "ProjectManager/Impl/ProjectManagerImpl.h"

namespace bv
{
// ********************************
//
ProjectManager::ProjectManager	()
	: m_impl( new ProjectManagerImpl() )
{}

// ********************************
//
ProjectManager::~ProjectManager	()
{
	delete m_impl;
}

// ********************************
//
ProjectManager *			ProjectManager::GetInstance		()
{
	static auto instance = new ProjectManager();

	return instance;
}

// ********************************
//
NormalizedPathVec			ProjectManager::ListProjectsNames	() const
{
	return m_impl->ListProjectsNames();
}

// ********************************
//
NormalizedPathVec			ProjectManager::ListScenesNames		( const NormalizedPath & projectName ) const
{
	return m_impl->ListScenesNames( projectName );
}

// ********************************
//
NormalizedPathVec			ProjectManager::ListCategoriesNames	() const
{
	return m_impl->ListCategoriesNames();
}

// ********************************
//
NormalizedPathVec			ProjectManager::ListAssetsPaths		( const NormalizedPath & projectName,  const std::string & categoryName ) const
{
	return m_impl->ListAssetsPaths( projectName, categoryName );
}

// ********************************
//
NormalizedPath				ProjectManager::GetRootDir			() const
{
	return m_impl->GetRootDir();
}

// ********************************
//
void						ProjectManager::AddNewProject		( const NormalizedPath & projectName )
{
	m_impl->AddNewProject( projectName );
}

// ********************************
//
const Project *				ProjectManager::GetProject			( const NormalizedPath & projectName ) const
{
	return m_impl->GetProject( projectName );
}

// ********************************
//
void						ProjectManager::SetCurrentProject	( const NormalizedPath & projectName )
{
	m_impl->SetCurrentProject( projectName );
}

// ********************************
//
void						ProjectManager::AddAsset			( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & path, const AssetDesc & assetDesc )
{
	m_impl->AddAsset( projectName, categoryName, path, assetDesc );
}

// ********************************
//
void						ProjectManager::CopyAsset			( const NormalizedPath & inProjectName, const std::string & inCategoryName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath )
{
	m_impl->CopyAsset( inProjectName, inCategoryName, inPath, outProjectName, outPath );
}

// ********************************
//
void						ProjectManager::RemoveAsset			( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & path )
{
	m_impl->RemoveAsset( projectName, categoryName, path );
}

// ********************************
//
void						ProjectManager::MoveAsset			( const NormalizedPath & inProjectName, const std::string & inCategoryName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath )
{
	m_impl->MoveAsset( inProjectName, inCategoryName, inPath, outProjectName, outPath );
}

// ********************************
//
void						ProjectManager::RemoveUnusedAssets	( const NormalizedPath & projectName, const std::string & categoryName )
{
	m_impl->RemoveUnusedAssets( projectName, categoryName );
}

// ********************************
//
void						ProjectManager::RemoveUnusedAssets	( const NormalizedPath & projectName )
{
	m_impl->RemoveUnusedAssets( projectName );
}

// ********************************
//
void						ProjectManager::AddScene			( const model::BasicNode & sceneRootNode, const NormalizedPath & projectName, const NormalizedPath & outPath )
{
	m_impl->AddScene( sceneRootNode, projectName, outPath );
}

// ********************************
//
void						ProjectManager::CopyScene			( const NormalizedPath & inProjectName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath )
{
	m_impl->CopyScene( inProjectName, inPath, outProjectName, outPath );
}

// ********************************
//
void						ProjectManager::RemoveScene			( const NormalizedPath & projectName, const NormalizedPath & path )
{
	m_impl->RemoveScene( projectName, path );
}

// ********************************
//
void						ProjectManager::MoveScene			( const NormalizedPath & inProjectName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath )
{
	m_impl->MoveScene( inProjectName, inPath, outProjectName, outPath );
}

// ********************************
//
void						ProjectManager::RegisterGlobalCategory( const Category & category)
{
	m_impl->RegisterGlobalCategory( category );
}

// ********************************
//
void						ProjectManager::ExportAssetToFile	( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & assetPath, const NormalizedPath & outputFile )
{
	m_impl->ExportAssetToFile( projectName, categoryName, assetPath, outputFile );
}

// ********************************
//
void						ProjectManager::ImportAssetFromFile	( const NormalizedPath & importToProjectName, const std::string & importToCategoryName, const NormalizedPath & importToPath, const NormalizedPath & importAssetFilePath )
{
	m_impl->ImportAssetFromFile( importToProjectName, importToCategoryName, importToPath, importAssetFilePath );
}
	
// ********************************
//
void						ProjectManager::ExportSceneToFile	( const NormalizedPath & projectName, const NormalizedPath & scenePath, const NormalizedPath & outputFile ) const
{
	m_impl->ExportSceneToFile( projectName, scenePath, outputFile );
}

// ********************************
//
void						ProjectManager::ImportSceneFromFile	( const NormalizedPath & importToProjectName, const NormalizedPath & importToPath, const NormalizedPath & impSceneFilePath )
{
	m_impl->ImportSceneFromFile( importToProjectName, importToPath, impSceneFilePath );
}

// ********************************
//
void						ProjectManager::ExportProjectToFile	( const NormalizedPath & projectName, const NormalizedPath & outputFilePath ) const
{
	m_impl->ExportProjectToFile( projectName, outputFilePath );
}

// ********************************
//
void						ProjectManager::ImportProjectFromFile( const NormalizedPath & expFilePath, const NormalizedPath & importToPath )
{
	m_impl->ImportProjectFromFile( expFilePath, importToPath );
}

// ********************************
//
AssetDesc *					ProjectManager::GetAssetDesc		( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & pathInProject ) const
{
	return m_impl->GetAssetDesc( projectName, categoryName, pathInProject );
}

// ********************************
//
SceneDesc *					ProjectManager::GetSceneDesc		( const NormalizedPath & projectName, const NormalizedPath & pathInProject ) const
{
	return m_impl->GetSceneDesc( projectName, pathInProject );
}

} // bv

