#include "ProjectManagerImpl.h"

namespace bv
{

// ********************************
//
ProjectManagerImpl::ProjectManagerImpl	()
{}

// ********************************
//
ProjectManagerImpl::~ProjectManagerImpl	()
{}

// ********************************
//
NormalizedPathVec			ProjectManagerImpl::ListProjectsNames	() const
{
}

// ********************************
//
NormalizedPathVec			ProjectManagerImpl::ListScenesNames		( const NormalizedPath & projectName ) const
{
}

// ********************************
//
NormalizedPathVec			ProjectManagerImpl::ListCategoriesNames	() const
{
}

// ********************************
//
NormalizedPathVec			ProjectManagerImpl::ListAssetsPaths		( const NormalizedPath & projectName,  const std::string & categoryName ) const
{
}

// ********************************
//
NormalizedPath				ProjectManagerImpl::GetRootDir			() const
{
}

// ********************************
//
void						ProjectManagerImpl::AddNewProject		( const NormalizedPath & projectName )
{
}

// ********************************
//
const Project *				ProjectManagerImpl::GetProject			( const NormalizedPath & projectName ) const
{
}

// ********************************
//
void						ProjectManagerImpl::SetCurrentProject	( const NormalizedPath & projectName )
{
}

// ********************************
//
void						ProjectManagerImpl::AddAsset			( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & path, const AssetDesc & assetDesc )
{
}

// ********************************
//
void						ProjectManagerImpl::CopyAsset			( const NormalizedPath & inProjectName, const std::string & inCategoryName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::RemoveAsset			( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & path )
{
}

// ********************************
//
void						ProjectManagerImpl::MoveAsset			( const NormalizedPath & inProjectName, const std::string & inCategoryName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::RemoveUnusedAssets	( const NormalizedPath & projectName, const std::string & categoryName )
{
}

// ********************************
//
void						ProjectManagerImpl::RemoveUnusedAssets	( const NormalizedPath & projectName )
{
}

// ********************************
//
void						ProjectManagerImpl::AddScene			( const model::BasicNode & sceneRootNode, const NormalizedPath & projectName, const NormalizedPath & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::CopyScene			( const NormalizedPath & inProjectName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::RemoveScene			( const NormalizedPath & projectName, const NormalizedPath & path )
{
}

// ********************************
//
void						ProjectManagerImpl::MoveScene			( const NormalizedPath & inProjectName, const NormalizedPath & inPath, const NormalizedPath & outProjectName, const NormalizedPath & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::RegisterGlobalCategory( const Category & category)
{
}

// ********************************
//
void						ProjectManagerImpl::ExportAssetToFile	( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & assetPath, const NormalizedPath & outputFile )
{
}

// ********************************
//
void						ProjectManagerImpl::ImportAssetFromFile	( const NormalizedPath & importToProjectName, const std::string & importToCategoryName, const NormalizedPath & importToPath, const NormalizedPath & importAssetFilePath )
{
}

// ********************************
//
void						ProjectManagerImpl::ExportSceneToFile	( const NormalizedPath & projectName, const NormalizedPath & scenePath, const NormalizedPath & outputFile ) const
{
}

// ********************************
//
void						ProjectManagerImpl::ImportSceneFromFile	( const NormalizedPath & importToProjectName, const NormalizedPath & importToPath, const NormalizedPath & impSceneFilePath )
{
}

// ********************************
//
void						ProjectManagerImpl::ExportProjectToFile	( const NormalizedPath & projectName, const NormalizedPath & outputFilePath ) const
{
}

// ********************************
//
void						ProjectManagerImpl::ImportProjectFromFile( const NormalizedPath & expFilePath, const NormalizedPath & importToPath )
{
}

// ********************************
//
AssetDesc *					ProjectManagerImpl::GetAssetDesc		( const NormalizedPath & projectName, const std::string & categoryName, const NormalizedPath & pathInProject ) const
{
}

// ********************************
//
SceneDesc *					ProjectManagerImpl::GetSceneDesc		( const NormalizedPath & projectName, const NormalizedPath & pathInProject ) const
{
}

} // bv