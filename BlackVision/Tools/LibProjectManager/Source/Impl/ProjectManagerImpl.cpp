#include "ProjectManagerImpl.h"

#include "Engine/Models/BVScene.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"

#include "Impl/Accessors/TextureAssetAccessor.h"
#include "Impl/Accessors/FontAssetAccessor.h"
#include "Impl/Accessors/AnimationAssetAccessor.h"

#include "IO/DirIO.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibProjectManager

#include <set>
#include <sstream>
#include <fstream>

namespace bv
{

// ********************************
//
ProjectManagerImpl::ProjectManagerImpl	( const Path & rootPath )
    : m_rootPath( rootPath.Absolute() )
	, m_projectsPath( m_rootPath / "projects" )
	, m_scenesPath( m_rootPath / "scenes" )
    , m_presetsPath( m_rootPath / "presets" )
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

	InitializeScenes();
    InitializePresets();
    InitializeAssets();
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
	auto pathInScenes = TranslateToPathCategory( projectName, "" );
	auto scenes = m_sceneAccessor->ListScenes( pathInScenes );

    for( auto & s : scenes )
    {
        s = Path( "scenes" ) / s;
    }

    return scenes;
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
			auto pathInCategory = TranslateToPathCategory( projectName, "" );
			auto cv = cit->second->ListAssets( pathInCategory );
            for( auto & p : cv )
            {
                p = Path( categoryName ) / p;
            }

            return cv;
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
			auto pathInCategory = TranslateToPathCategory( projectName, "" );
			auto cv = c.second->ListAssets( pathInCategory );

            for( auto & p : cv )
            {
                p = Path( c.second->GetId() ) / p;
            }

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
        auto p = Project::Create( projectName, m_projectsPath );
        if( Path::Exists( m_projectsPath / projectName ) )
        {
            m_projects[ projectName.Str() ] = p;
        }
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
		auto pInCategory = TranslateToPathCategory( projectName, path );
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

		auto pInCategory = TranslateToPathCategory( outProjectName, outPath );
		cit->second->AddAsset( pInCategory, a );
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Asset '" << ( inProjectName / inCategoryName / inPath ).Str() << "' doesn't exist.";
	}
}

// ********************************
//
void						ProjectManagerImpl::RemoveAsset			( const Path & projectName, const std::string & categoryName, const Path & path ) const
{
	auto cit = m_categories.find( categoryName );

	if( cit != m_categories.end() )
	{
		auto pInCategory = TranslateToPathCategory( projectName, path );
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
void						ProjectManagerImpl::RemoveUnusedAssets	( const Path & projectName, const std::string & categoryName ) const
{
    auto it = m_categories.find( categoryName );

    if( GetProject( projectName ) )
    {
        if( it != m_categories.end() )
        {
            auto assetsInCategory = it->second->ListAssets( projectName );

            auto scenes = m_sceneAccessor->ListScenes( "" );

            std::set< Path > usedAssets;

            for( auto s : scenes )
            {
                auto as = m_sceneAccessor->ListAllUsedAssets( s );
                usedAssets.insert( as.begin(), as.end() );
            }

            for( auto aInCat : assetsInCategory )
            {
                if( usedAssets.find( Path( categoryName ) / aInCat ) == usedAssets.end() )
                {
                    RemoveAsset( "", categoryName, aInCat );
                }
            }
        }
    }    
}

// ********************************
// 
void						ProjectManagerImpl::RemoveUnusedAssets	( const Path & projectName ) const
{
    if( GetProject( projectName ) )
    {
        for( auto k : m_categories )
        {
            RemoveUnusedAssets( projectName, k.first );
        }
    }
}

// ********************************
//
void						ProjectManagerImpl::RemoveUnusedAssets	() const
{
    for( auto p : m_projects )
    {
        for( auto k : m_categories )
        {
            RemoveUnusedAssets( p.second->GetName(), k.first );
        }
    }
}

// ********************************
//
void						ProjectManagerImpl::AddScene			( const model::BasicNodeConstPtr & scene, const Path & projectName, const Path & outPath )
{
	auto pathInScenes = TranslateToPathCategory( projectName, outPath );

	m_sceneAccessor->AddScene( scene, pathInScenes );
}

// ********************************
//
void						ProjectManagerImpl::CopyScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
	auto inPathInScenes = TranslateToPathCategory( inProjectName, inPath );
	auto outPathInScenes = TranslateToPathCategory( outProjectName, outPath );

	auto sceneDesc = m_sceneAccessor->GetSceneDesc( inPathInScenes );

	Path::Copy( sceneDesc.GetPath(), outPathInScenes );
}

// ********************************
//
void						ProjectManagerImpl::RemoveScene			( const Path & projectName, const Path & path )
{
	auto pathInScenes = TranslateToPathCategory( projectName, path );

	Path::Remove( m_rootPath / pathInScenes );
}

// ********************************
//
void						ProjectManagerImpl::MoveScene			( const Path & inProjectName, const Path & inPath, const Path & outProjectName, const Path & outPath )
{
	CopyScene( inProjectName, inPath, outProjectName, outPath );
	RemoveScene( inProjectName, inPath );
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
    auto f = File::Open( outputFile.Str(), File::OpenMode::FOMReadWrite );

    auto & out = *f.StreamBuf();

    out << "assets" << '\n';

    auto uniqueAssets = m_sceneAccessor->ListAllUsedAssets( projectName / scenePath );

    out << std::to_string( uniqueAssets.size() ) << '\n';

    for( auto ua : uniqueAssets)
    {
        if( IsExternalPath( ua ) )
        {
            continue;
        }

        auto loc = Path2Location( ua );

        if( loc.categoryName == "scenes" ) 
	        assert( false );

        out << loc.categoryName << '\n';

        out << loc.path << '\n';

        m_categories.at( loc.categoryName )->ExportAsset( out, loc.projectName / loc.path );
    }

    out << '\n';

    m_sceneAccessor->ExportScene( out, projectName, scenePath );
  	f.Close();
}

// ********************************
//
void						ProjectManagerImpl::ImportSceneFromFile	( const Path & importToProjectName, const Path & importToPath, const Path & impSceneFilePath )
{
    auto f = File::Open( impSceneFilePath.Str() );

    std::stringbuf buf;

    auto & in = *f.StreamBuf();

    in.get( buf, '\n');
    in.ignore();

    if( buf.str() == "assets" )
    {
        std::stringbuf buf;
        in.get( buf, '\n');
        in.ignore();

        auto size = stoul( buf.str() );

        for( SizeType i = 0; i < size; ++i )
        {
            std::stringbuf buf;
            in.get( buf, '\n');
            in.ignore();

            auto categoryName = buf.str();
            buf.str("");

            in.get( buf, '\n');
            in.ignore();
            auto path = Path( buf.str() );

            m_categories.at( categoryName )->ImportAsset( in, importToProjectName / importToPath );
        }
    }
    else
    {
        in.seekg( 0 );
    }

	m_sceneAccessor->ImportSceneFromFile( impSceneFilePath, importToProjectName, importToPath );
}

// ********************************
//
void						ProjectManagerImpl::ExportProjectToFile	( const Path & projectName, const Path & outputFilePath ) const
{
	auto project = GetProject( projectName );

	if( project )
	{
		auto projectAssets = ListAssetsPaths( projectName );
		auto projectScenes = ListScenesNames( projectName );

		std::set< Path > uniqueAssets;

		uniqueAssets.insert( projectAssets.begin(), projectAssets.end() );

		for( auto ps : projectScenes )
		{
            auto loc = Path2Location( ps );
            auto sa = m_sceneAccessor->ListAllUsedAssets( loc.projectName / loc.path );
			uniqueAssets.insert( sa.begin(), sa.end() );
		}

        auto assetsFile = File::Open( outputFilePath.Str(), File::OpenMode::FOMReadWrite );

		auto & out = *assetsFile.StreamBuf();

        out << "assets" << '\n';

        out << std::to_string( uniqueAssets.size() ) << '\n';

		for( auto ua : uniqueAssets)
		{
            if( IsExternalPath( ua ) )
            {
                continue;
            }

			auto loc = Path2Location( ua );

			if( loc.categoryName == "scenes" ) 
				assert( false );

            out << loc.categoryName << '\n';

            out << loc.path << '\n';

			m_categories.at( loc.categoryName )->ExportAsset( out, loc.projectName / loc.path );
		}

        out << '\n';
        
        out << "scenes" << '\n';

        out << std::to_string( projectScenes.size() ) << '\n';

		for( auto s : projectScenes )
		{
            auto loc = Path2Location( s );

            out << loc.path << '\n';

			m_sceneAccessor->ExportScene( out, loc.projectName, loc.path );
		}

        assetsFile.Close();
	}
	else
	{
		LOG_MESSAGE( SeverityLevel::error ) << "Cannot export project '" << projectName << "'. It doesn't exist.";
	}
}

// ********************************
//
void						ProjectManagerImpl::ImportProjectFromFile( const Path & expFilePath, const Path & projectName )
{
    AddNewProject( projectName );

    auto f = File::Open( expFilePath.Str() );

    std::stringbuf buf;

    auto & in = *f.StreamBuf();

    in.get( buf, '\n');
    in.ignore();

    if( buf.str() == "assets" )
    {
        std::stringbuf buf;
        in.get( buf, '\n');
        in.ignore();

        auto size = stoul( buf.str() );

        for( SizeType i = 0; i < size; ++i )
        {
            std::stringbuf buf;
            in.get( buf, '\n');
            in.ignore();

            auto categoryName = buf.str();
            buf.str("");

            in.get( buf, '\n');
            in.ignore();
            auto path = Path( buf.str() );

            m_categories.at( categoryName )->ImportAsset( in, projectName / path );
        }
    }
    else
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Cannot import project '" << projectName << "'. Wrong format.";
    }

    buf.str("");

    in.ignore(); // ignoring new line after asset data

    in.get( buf, '\n');
    in.ignore();
    if( buf.str() == "scenes" )
    {
        buf.str("");
        in.get( buf, '\n');
        in.ignore();

        SizeType size = stoul( buf.str() );

        for( SizeType i = 0; i < size; ++i )
        {
            buf.str("");
            in.get( buf, '\n');
            in.ignore();
            Path path = buf.str();

            m_sceneAccessor->ImportScene( in, projectName, path );
        }
    }

    f.Close();
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
SceneDescriptor				ProjectManagerImpl::GetSceneDesc		( const Path & projectName, const Path & pathInProject ) const
{
	auto pathInCategory = TranslateToPathCategory( projectName, pathInProject );
	return m_sceneAccessor->GetSceneDesc( pathInCategory );
}

// ********************************
//
SceneDescriptor			    ProjectManagerImpl::GetSceneDesc		( const Path & path ) const
{
    auto loc = Path2Location( path );
    auto pathInCategory = TranslateToPathCategory( loc.projectName, loc.path );
	return m_sceneAccessor->GetSceneDesc( pathInCategory );
}


// ********************************
//
void						ProjectManagerImpl::InitializeProjects	()
{
	if( Path::Exists( m_projectsPath ) )
	{
		auto l = Path::List( m_projectsPath, true, "\\.bvproj" );

		for( auto p : l )
		{
			auto n = Path::RelativePath( p, m_projectsPath );
            auto newProjectName = n.ParentPath();
            AddNewProject( newProjectName );
		}
	}
	else
	{
		Dir::CreateDir( m_projectsPath.Str() );
	}
}

// ********************************
//
void						ProjectManagerImpl::InitializeScenes	()
{
	if( !Path::Exists( m_scenesPath ) )
	{
		Dir::CreateDir( m_scenesPath.Str() );
	}

    m_sceneAccessor = SceneAccessor::Create( m_rootPath );
}

// ********************************
//
void						ProjectManagerImpl::InitializePresets	()
{
	if( !Path::Exists( m_presetsPath ) )
	{
		Dir::CreateDir( m_presetsPath.Str() );
	}

    m_presetAccessor = PresetAccessor::Create( m_presetsPath );
}

// ********************************
//
void				        ProjectManagerImpl::InitializeAssets	()
{
	StringVector exts;
	exts.push_back( ".*\\.jpg" );
	exts.push_back( ".*\\.tga" );
	exts.push_back( ".*\\.png" );

	auto taa = TextureAssetAccessor::Create( GetRootDir() / "textures", exts );
	RegisterCategory( AssetCategory::Create( "textures", taa ) );

    auto aaa = AnimationAssetAccessor::Create( GetRootDir() / "sequences", exts );
	RegisterCategory( AssetCategory::Create( "sequences", aaa ) );

    StringVector fontsExts;
	fontsExts.push_back( ".*\\.ttf" );

    auto faa = FontAssetAccessor::Create( GetRootDir() / "fonts", fontsExts );
	RegisterCategory( AssetCategory::Create( "fonts", faa ) );
}

// ********************************
//
Path						ProjectManagerImpl::TranslateToPathCategory			( const Path & projectName, const Path & path ) const
{
	if( !projectName.Str().empty() )
	{
		if( projectName.Str() == "." )
		{
			if( m_currentProject )
			{
				return m_currentProject->GetName() / path;
			}
			else
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Current project's not set.";
                return "";
			}
		}
		else
		{
			auto p = GetProject( projectName );
			if( p )
			{
				return projectName / path;
			}
			else
			{
				LOG_MESSAGE( SeverityLevel::error ) << "Project '" << projectName.Str() << "' doesn't exist.";
				return "";
			}
		}
	}
    else
    {
        return path;
    }
}

// ********************************
//
Path						ProjectManagerImpl::TranslateToPathInPMRootFolder( const Path & projectName, const std::string & categoryName, const Path & path ) const
{
	auto ret = TranslateToPathCategory( projectName, path );

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

// ********************************
//
ProjectManagerImpl::Location ProjectManagerImpl::Path2Location( const Path & path ) const
{
    if( path.Str().empty() )
    {
        return Location();
    }

	auto strPath = path.Str();

	auto categoriesNames = ListCategoriesNames();

	categoriesNames.push_back( "scenes" ); // Adding scenes to categories

	std::string categoryName = "";

	for( auto cn : categoriesNames )
	{
		auto pos = strPath.find( cn );
		if( pos == 0 )
		{
			categoryName = cn;
			break;
		}
	}

    if( categoryName.empty() )
    {
        LOG_MESSAGE( SeverityLevel::warning ) << "'" << path << "' is not valid path inside project manager.";
        return Location();
    }

	Path projectName = "";

	for( auto pn : ListProjectsNames() )
	{
		auto pos = strPath.find( "\\" + pn.Str() + "\\" );
		if( pos == categoryName.size() )
		{
			projectName = pn;
			break;
		}
	}

	Location lok = { categoryName, projectName, strPath.substr( projectName.Str().size() + categoryName.size() + 2 ) };
	
	return lok;
}

// ********************************
//
Path						ProjectManagerImpl::Location2Path( const Location & loc ) const
{
	return Path( loc.categoryName ) / loc.projectName / loc.path;
}

// ********************************
//
model::BasicNodeConstPtr    ProjectManagerImpl::LoadPreset          ( const Path & projectName, const Path & path ) const
{
    auto pathInCategory = TranslateToPathCategory( projectName, path );
    return m_presetAccessor->LoadPreset( pathInCategory );
}

// ********************************
//
void                        ProjectManagerImpl::SavePreset          ( const model::BasicNodeConstPtr & node, const Path & projectName, const Path & path ) const
{
    auto pathInCategory = TranslateToPathCategory( projectName, path );
    return m_presetAccessor->SavePreset( node, pathInCategory );
}

// ********************************
//
PathVec                     ProjectManagerImpl::ListPresets         ( const Path & projectName, const Path & path ) const
{
    auto pathInCategory = TranslateToPathCategory( projectName, path );
    return m_presetAccessor->ListPresets( pathInCategory );
}

// ********************************
//
PathVec                     ProjectManagerImpl::ListPresets         ( const Path & projectName ) const
{
    auto pathInCategory = TranslateToPathCategory( projectName, "" );
    return m_presetAccessor->ListPresets( pathInCategory );
}

// ********************************
//
PathVec                     ProjectManagerImpl::ListPresets         () const
{
    return m_presetAccessor->ListPresets( "" );
}

// ********************************
//
Path                        ProjectManagerImpl::ToAbsPath           ( const Path & path ) const
{
    if( path.Str().find( "file:/" ) == 0 )
    {
        return path.Str().substr( 6 );
    }
    else if ( path.Str().find( "seq:/" ) == 0 )
    {
        return path.Str().substr( 4 );
    }
    else if ( path.Str().find( "stream:/" ) == 0 )
    {
        return path.Str().substr( 8 );
    }
    else
    {
        if( IsValidPMPath( path ) )
        {
            return m_rootPath / path;
        }
        else
        {
            // FIXME: This else is needed only for testing old mechanism with pablito's solutions
            return path;
        }
    }
}

// ********************************
//
bool                        ProjectManagerImpl::IsExternalPath      ( const Path & path ) const
{
    if( path.Str().find( "file:/" ) == 0 )
    {
        return true;
    }
    else if ( path.Str().find( "seq:/" ) == 0 )
    {
        return true;
    }
    else if ( path.Str().find( "stream:/" ) == 0 )
    {
        return true;
    }
    else
    {
        return IsValidPMPath( path );
    }
}

// ********************************
//
bool                        ProjectManagerImpl::IsValidPMPath       ( const Path & path ) const
{
    return Path2Location( path ).IsValid();
}

// ********************************
//
bool                        ProjectManagerImpl::PathExistsInPM      ( const Path & path ) const
{
    if( !IsExternalPath( path ) )
    {
        return Path::Exists( ToAbsPath( path ) );
    }
    else
    {
        return false;
    }
}

} // bv