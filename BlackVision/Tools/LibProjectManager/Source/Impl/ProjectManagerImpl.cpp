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
	PathVec ret;
	for( auto it : m_projects )
	{
		ret.push_back( it.first );
	}

	return ret;
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
StringVector	ProjectManagerImpl::ListCategoriesNames	() const
{
	StringVector ret;
	for( auto it : m_categories )
	{
		ret.push_back( it.first );
	}

	return ret;
}

// ********************************
//
PathVec			ProjectManagerImpl::ListAssetsPaths		( const Path & projectName,  const std::string & categoryName ) const
{
	if( !categoryName.empty() )
	{
		auto cit = m_categories.find( categoryName );

		if( cit != m_categories.end() )
		{
			auto pathInCategory = TranslateToPathCaegory( projectName, "" );
			return cit->second->ListAssets( pathInCategory );
		}
		else
		{
			LOG_MESSAGE( SeverityLevel::error ) << "Category '" << categoryName << " doesn't exist.";
			return PathVec();
		}
	}
	else
	{
		PathVec ret;
		for( auto c : m_categories )
		{
			auto pathInCategory = TranslateToPathCaegory( projectName, "" );
			auto cv = c.second->ListAssets( pathInCategory );
			ret.insert( ret.end(), cv.begin(), cv.end() );
		}
		return ret;
	}
}

// ********************************
//
Path				ProjectManagerImpl::GetRootDir			() const
{
	return m_rootPath;
}

// ********************************
//
void						ProjectManagerImpl::AddNewProject		( const Path & projectName )
{
	auto it = m_projects.find( projectName.Str() );
	
	if( it == m_projects.end() )
	{
		m_projects[ projectName.Str() ] = Project::Create( projectName, m_projectsPath );
	}
}

// ********************************
//
ProjectConstPtr				ProjectManagerImpl::GetProject			( const Path & projectName ) const
{
	auto it = m_projects.find( projectName.Str() );
	if( it != m_projects.end() )
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

// ********************************
//
void						ProjectManagerImpl::SetCurrentProject	( const Path & projectName )
{
	auto p = GetProject( projectName );
	if( p )
	{
		m_currentProject = p;
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannot set current project: " << projectName.Str() << ". It doesn't exist.";
	}
}

// ********************************
//
void						ProjectManagerImpl::AddAsset			( const Path & projectName, const std::string & categoryName, const Path & path, const AssetDescConstPtr & assetDesc )
{
	auto cit = m_categories.find( categoryName );

	if( cit != m_categories.end() )
	{
		auto pInCategory = TranslateToPathCaegory( projectName, path );
		cit->second->AddAsset( pInCategory, assetDesc );
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannot add asset to category '" << categoryName << "'. Category doesn't exist.";
	}
}

// ********************************
//
void						ProjectManagerImpl::CopyAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
	auto a = GetAssetDesc( inProjectName, inCategoryName, inPath );

	if( a )
	{
		auto cit = m_categories.find( inCategoryName );

		auto pInCategory = TranslateToPathCaegory( outProjectName, outPath );
		cit->second->AddAsset( pInCategory, a );
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Asset '" << ( inProjectName / inCategoryName / inPath ).Str() << "' doesn't exist.";
	}
}

// ********************************
//
void						ProjectManagerImpl::RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path )
{
	auto cit = m_categories.find( categoryName );

	if( cit != m_categories.end() )
	{
		auto pInCategory = TranslateToPathCaegory( projectName, path );
		cit->second->RemoveAsset( pInCategory );
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannot remove. Category " << categoryName << " doesn't exist.";
	}
}

// ********************************
//
void						ProjectManagerImpl::MoveAsset			( const Path & inProjectName, const std::string & inCategoryName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
	CopyAsset( inProjectName, inCategoryName, inPath, outProjectName, outPath );
	RemoveAsset( inProjectName, inCategoryName, inPath );
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
void						ProjectManagerImpl::AddScene			( const model::BasicNode & , const Path & projectName, const Path & outPath )
{
	{projectName;}
	{outPath;}
}

// ********************************
//
void						ProjectManagerImpl::CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
	{inProjectName;}
	{inPath;}
	{outProjectName;}
	{outPath;}
}

// ********************************
//
void						ProjectManagerImpl::RemoveScene			( const Path & projectName, const Path & path )
{
	{projectName;}
	{path;}
}

// ********************************
//
void						ProjectManagerImpl::MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
	{inProjectName;}
	{inPath;}
	{outProjectName;}
	{outPath;}
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
	{projectName;}
	{scenePath;}
	{outputFile;}
}

// ********************************
//
void						ProjectManagerImpl::ImportSceneFromFile	( const Path & importToProjectName, const Path & importToPath, const Path & impSceneFilePath )
{
	{importToProjectName;}
	{importToPath;}
	{impSceneFilePath;}
}

// ********************************
//
void						ProjectManagerImpl::ExportProjectToFile	( const Path & projectName, const Path & outputFilePath ) const
{
	{projectName;}
	{outputFilePath;}
}

// ********************************
//
void						ProjectManagerImpl::ImportProjectFromFile( const Path & expFilePath, const Path & importToPath )
{
	{expFilePath;}
	{importToPath;}
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
	{projectName;}
	{pathInProject;}
	return nullptr;
}

// ********************************
//
void						ProjectManagerImpl::InitializeProjects	()
{
	if( Path::Exists( m_projectsPath ) )
	{
		auto l = Path::List( m_projectsPath, ".*bvproj" );

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

// ********************************
//
Path						ProjectManagerImpl::TranslateToPathCaegory			( const Path & projectName, const Path & path ) const
{
	Path ret;

	if( !projectName.Str().empty() )
	{
		if( projectName.Str() == "." )
		{
			if( m_currentProject )
			{
				ret = ret / m_currentProject->GetName();
			}
			else
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Current project's not set.";
			}
		}
		else
		{
			auto p = GetProject( projectName );
			if( p )
			{
				ret = ret / projectName;
			}
			else
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Project '" << projectName.Str() << "' doesn't exist.";
				return ret;
			}
		}
	}

	return ret / path;
}

// ********************************
//
Path						ProjectManagerImpl::TranslateToPathInPMRootFolder( const Path & projectName, const std::string & categoryName, const Path & path ) const
{
	auto ret = TranslateToPathCaegory( projectName, path );

	if( !categoryName.empty() )
	{
		return Path( categoryName ) / ret;
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Category name cannot be empty.";
		return Path();
	}
}

} // bv