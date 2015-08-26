#include "ProjectManagerImpl.h"

#include "IO/DirIO.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibProjectManager

namespace bv
{

// ********************************
//
ProjectManagerImpl::ProjectManagerImpl	( const Path & rootPath )
	: m_rootPath( rootPath )
	, m_projectsPath( m_rootPath / Path( "projects" ) )
{
	if( Path::Exists( rootPath ) )
	{
		InitializeProjects();
	}
	else
	{
		Dir::CreateDir( rootPath.Str(), true );
		Dir::CreateDir( m_projectsPath.Str() );
	}
}

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
	{projectName;}
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
	{projectName;}{categoryName;}
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
	auto it = m_projects.find( projectName.Str() );
	
	if( it == m_projects.end() )
	{
		m_projects[ projectName.Str() ] = Project::Create( projectName, GetRootDir() );
	}
}

// ********************************
//
const Project *				ProjectManagerImpl::GetProject			( const Path & projectName ) const
{
	{projectName;}
	return nullptr;
}

// ********************************
//
void						ProjectManagerImpl::SetCurrentProject	( const Path & projectName )
{
	{projectName;}
}

// ********************************
//
void						ProjectManagerImpl::AddAsset			( const Path & projectName, const std::string & categoryName, const Path & path, const AssetDescConstPtr & assetDesc )
{
	{projectName;}
	{categoryName;}
	{path;}
	{assetDesc;}
}

// ********************************
//
void						ProjectManagerImpl::CopyAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
	{inProjectName;}
	{inCategoryName;}
	{inPath;}
	{outProjectName;}
	{outPath;}
}

// ********************************
//
void						ProjectManagerImpl::RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path )
{
	{projectName;}
	{categoryName;}
	{path;}
}

// ********************************
//
void						ProjectManagerImpl::MoveAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
	{inProjectName;}
	{inCategoryName;}
	{inPath;}
	{outProjectName;}
	{outPath;}
}

// ********************************
//
void						ProjectManagerImpl::RemoveUnusedAssets	( const Path & projectName, const std::string & categoryName )
{
	{projectName;}
	{categoryName;}
}

// ********************************
//
void						ProjectManagerImpl::RemoveUnusedAssets	( const Path & projectName )
{
	{projectName;}
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
void						ProjectManagerImpl::RegisterCategory( const AssetCategoryConstPtr & category )
{
	auto it = m_categories.find( category->GetId() );

	if( it == m_categories.end() )
	{
		m_categories[ category->GetId() ] = category;
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannot register category: " << category->GetId() << ". Already registered.";
	}
}

// ********************************
//
void						ProjectManagerImpl::ExportAssetToFile	( const Path & projectName, const std::string & categoryName, const Path & assetPath, const Path & outputFile )
{
	auto it = m_categories.find( categoryName );
	
	if( it != m_categories.end() )
	{
		it->second->ExportAsset( outputFile, projectName / assetPath );
	}
}

// ********************************
//
void						ProjectManagerImpl::ImportAssetFromFile	( const Path & importToProjectName, const std::string & importToCategoryName, const Path & importToPath, const Path & importAssetFilePath )
{
	auto it = m_categories.find( importToCategoryName );
	
	if( it != m_categories.end() )
	{
		it->second->ImportAsset( importAssetFilePath, importToProjectName / importToPath );
	}
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
	auto it = m_categories.find( categoryName );
	
	if( it != m_categories.end() )
	{
		return it->second->GetAssetDesc( projectName / pathInProject );
	}
	else
	{
		return AssetDescConstPtr();
	}
}

// ********************************
//
SceneDesc *					ProjectManagerImpl::GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const
{
	return nullptr;
}

// ********************************
//
void						ProjectManagerImpl::InitializeProjects	()
{
	if( Path::Exists( m_projectsPath ) )
	{
		auto l = Path::List( m_projectsPath, "*./.bvproj" );

		for( auto p : l )
		{
			auto n = Path::RelativePath( p, m_projectsPath );
			AddNewProject( n );
		}
	}
	else
	{
		Dir::CreateDir( m_projectsPath.Str() );
	}
}

} // bv