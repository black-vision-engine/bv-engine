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
PathVec			ProjectManagerImpl::ListProjectsNames	() const
{
	return PathVec();
}

// ********************************
//
PathVec			ProjectManagerImpl::ListScenesNames		( const Path & projectName ) const
{
	return PathVec();
}

// ********************************
//
PathVec			ProjectManagerImpl::ListCategoriesNames	() const
{
	return PathVec();
}

// ********************************
//
PathVec			ProjectManagerImpl::ListAssetsPaths		( const Path & projectName,  const std::string & categoryName ) const
{
	return PathVec();
}

// ********************************
//
Path				ProjectManagerImpl::GetRootDir			() const
{
	return Path( "" );
}

// ********************************
//
void						ProjectManagerImpl::AddNewProject		( const Path & projectName )
{
}

// ********************************
//
const Project *				ProjectManagerImpl::GetProject			( const Path & projectName ) const
{
	return nullptr;
}

// ********************************
//
void						ProjectManagerImpl::SetCurrentProject	( const Path & projectName )
{
}

// ********************************
//
void						ProjectManagerImpl::AddAsset			( const Path & projectName, const std::string & categoryName, const Path & path, const AssetDescConstPtr & assetDesc )
{
}

// ********************************
//
void						ProjectManagerImpl::CopyAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path )
{
}

// ********************************
//
void						ProjectManagerImpl::MoveAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::RemoveUnusedAssets	( const Path & projectName, const std::string & categoryName )
{
}

// ********************************
//
void						ProjectManagerImpl::RemoveUnusedAssets	( const Path & projectName )
{
}

// ********************************
//
void						ProjectManagerImpl::AddScene			( const model::BasicNode & sceneRootNode, const Path & projectName, const Path & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::RemoveScene			( const Path & projectName, const Path & path )
{
}

// ********************************
//
void						ProjectManagerImpl::MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
}

// ********************************
//
void						ProjectManagerImpl::RegisterGlobalCategory( const Category & category)
{
}

// ********************************
//
void						ProjectManagerImpl::ExportAssetToFile	( const Path & projectName, const std::string & categoryName, const Path & assetPath, const Path & outputFile )
{
}

// ********************************
//
void						ProjectManagerImpl::ImportAssetFromFile	( const Path & importToProjectName, const std::string & importToCategoryName, const Path & importToPath, const Path & importAssetFilePath )
{
}

// ********************************
//
void						ProjectManagerImpl::ExportSceneToFile	( const Path & projectName, const Path & scenePath, const Path & outputFile ) const
{
}

// ********************************
//
void						ProjectManagerImpl::ImportSceneFromFile	( const Path & importToProjectName, const Path & importToPath, const Path & impSceneFilePath )
{
}

// ********************************
//
void						ProjectManagerImpl::ExportProjectToFile	( const Path & projectName, const Path & outputFilePath ) const
{
}

// ********************************
//
void						ProjectManagerImpl::ImportProjectFromFile( const Path & expFilePath, const Path & importToPath )
{
}

// ********************************
//
AssetDescConstPtr			ProjectManagerImpl::GetAssetDesc		( const Path & projectName, const std::string & categoryName, const Path & pathInProject ) const
{
	return AssetDescConstPtr();
}

// ********************************
//
SceneDesc *					ProjectManagerImpl::GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const
{
	return nullptr;
}

} // bv