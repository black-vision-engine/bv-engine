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

} // bv

