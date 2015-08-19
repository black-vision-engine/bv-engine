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
StringVector				ProjectManager::ListProjectsNames	() const
{
	// TODO: Implement
}

// ********************************
//
StringVector				ProjectManager::ListScenesNames		( const std::string & projectName ) const
{
	// TODO: Implement
}

// ********************************
//
StringVector				ProjectManager::ListCategoriesNames	() const
{
	// TODO: Implement
}

// ********************************
//
StringVector				ProjectManager::ListAssetsPaths		( const std::string & projectName,  const std::string & categoryName ) const
{
	// TODO: Implement
}

// ********************************
//
std::string					ProjectManager::GetRootDir			() const
{
	// TODO: Implement
}

// ********************************
//
void						ProjectManager::AddNewProject		( const std::string & projectName )
{
	// TODO: Implement
}

// ********************************
//
const Project *				ProjectManager::GetProject			() const
{
	// TODO: Implement
}

// ********************************
//
void						ProjectManager::SetCurrentProject	( const std::string & projectName )
{
	// TODO: Implement
}

// ********************************
//
void						ProjectManager::AddAsset			( const std::string & projectName, const std::string & categoryName, const std::string & path, const AssetDesc & assetDesc )
{
	// TODO: Implement
}

} // bv

