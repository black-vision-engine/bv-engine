#include "SceneAccessor.h"

#include "Engine/Models/Plugins/Plugin.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibBlackVision

#include <set>

namespace bv
{

const std::string SceneAccessor::SceneFileExt = ".*\\.scn";
extern model::TimelineManager * global_tm;

// ********************************
//
SceneAccessorConstPtr SceneAccessor::Create( const Path & path, model::TimelineManager * tm, const model::PluginsManager * pm )
{
	return SceneAccessorConstPtr( new SceneAccessor( path, tm, pm ) );
}

// ********************************
//
SceneAccessor::SceneAccessor( const Path & path, model::TimelineManager * tm, const model::PluginsManager * pm )
	: m_rootDirPM( path )
    , m_rootDir( path / "scenes" )
	, m_tm( tm )
	, m_pm( pm )
{}

// ********************************
//
SceneDescriptor	SceneAccessor::GetSceneDesc( const Path & path ) const
{
	if( Path::Exists( m_rootDir / path ) )
	{
		return SceneDescriptor( m_rootDir / path, m_tm, m_pm );
	}
	else
	{
		return SceneDescriptor( "", m_tm, m_pm  );
	}
}

// ********************************
//
model::BasicNodeConstPtr	SceneAccessor::GetScene( const Path & path ) const
{
	auto desc = GetSceneDesc( path );
	return desc.LoadScene();
}

// ********************************
//
void			SceneAccessor::AddSceneFromFile( const Path & srcPath, const Path & path ) const
{
	Path::Copy( srcPath, path );
}

// ********************************
//
void			SceneAccessor::AddScene( const model::BasicNodeConstPtr & scene, const Path & path ) const
{
	SceneDescriptor::SaveScene( scene, m_rootDir / path );
}

// ********************************
//
void			SceneAccessor::RemoveScene( const Path & path ) const
{
	Path::Remove( m_rootDir / path );
}

// ********************************
//
void			SceneAccessor::ImportScene( std::istream & in, const Path & importToPath ) const
{
    std::stringbuf buf;
    in.get( buf, '\n' );
    in.ignore();

    if( buf.str() == "serialized_scene_begin" )
    {    
        buf.str("");
        in.get( buf, '\n' );
        in.ignore();
        Path oldPMRootDir = buf.str();

	    auto f = File::Open( ( m_rootDir / importToPath ).Str(), File::OpenMode::FOMReadWrite );
        rapidxml::xml_document<> doc;

        buf.str("");
        in.get( buf, '\n' );
        auto size = stoul( buf.str() );
        in.ignore();
        //char * cbuf = new char[ size ];

        //in.read( cbuf, size );

        //std::string tmpFileName;
        //auto tmpFile = File::OpenTmp( &tmpFileName );
        //tmpFile.Write( cbuf, size );
        //tmpFile.Close();

        //auto desc = SceneDescriptor( tmpFileName, bv::global_tm, &model::PluginsManager::DefaultInstanceRef() );
        //auto scene = desc.LoadScene();

        //auto docNode = doc.first_node( "scene" );

	    f.Write( in, size );

        f.Close();

        buf.str( "" );
        in.get( buf, '\n' );
        in.ignore();

        if( buf.str() != "serialized_scene_end" )
        {
            LOG_MESSAGE_FILE_LINE( SeverityLevel::error ) << "Cannot import scene: '" << importToPath.Str() << "'. Wrong format.";
        }
    }
}

// ********************************
//
void			SceneAccessor::ExportScene( std::ostream & out, const Path & path, bool withAssets ) const
{
	if( !withAssets )
	{
        out << "serialized_scene_begin" << '\n';

        out << m_rootDirPM << '\n';

		SceneDescriptor sceneDesc( m_rootDir / path, m_tm, m_pm );

		auto scene = sceneDesc.LoadScene();

		auto sob = new SerializeObject();

		sob->SetName( "scene" );
		scene->Serialize( *sob );
		sob->Pop();

        std::stringstream serScene;

		sob->Save( serScene );

        auto serSceneString = serScene.str();

        out << std::to_string( serSceneString.size() ) << '\n';

        out << serSceneString;

        out << "serialized_scene_end" << '\n';
	}
	else
	{
		assert( !"Not implemented." );
	}
}

// ********************************
//
void			SceneAccessor::ImportSceneFromFile( const Path & expFilePath, const Path & importToPath ) const
{
	auto f = File::Open( expFilePath.Str(), File::OpenMode::FOMReadOnly );
	auto in = f.StreamBuf();
	ImportScene( *in, importToPath );
	f.Close();
}

// ********************************
//
void			SceneAccessor::ExportSceneToFile( const Path & outputFileName, const Path & path, bool withAssets ) const
{
	auto f = File::Open( outputFileName.Str(), File::OpenMode::FOMReadOnly );
	auto out = f.StreamBuf();
	ExportScene( *out, path, withAssets );
	f.Close();
}

// ********************************
//
PathVec			SceneAccessor::ListScenes( const Path & path ) const
{
	auto sceneList = Path::List( m_rootDir / path, SceneFileExt );

    for( auto & s : sceneList )
    {
        s = Path::RelativePath( s, m_rootDir );
    }

    return sceneList;
}

// ********************************
//
PathVec			SceneAccessor::ListAllUsedAssets( const Path & path ) const
{
	SceneDescriptor desc( m_rootDir / path, m_tm, m_pm  );
	auto scene = desc.LoadScene();


	return GetAllUsedAssetPaths( scene, m_rootDirPM );
}

// ********************************
//
PathVec			SceneAccessor::GetAllUsedAssetPaths( const model::BasicNodeConstPtr & scene, const Path & relativeTo )
{
	auto rootNode = std::const_pointer_cast< model::BasicNode >( scene );

    auto lastPlugin = rootNode->GetPlugins()->GetLastPlugin();

	std::vector< AssetDescConstPtr > allDescs;

	while( lastPlugin )
	{
		auto assetsDescs = std::static_pointer_cast< model::BasePlugin< model::IPlugin > >( lastPlugin )->GetAssets();
		allDescs.insert( allDescs.end(), assetsDescs.begin(), assetsDescs.end() );
        lastPlugin = lastPlugin->GetPrevPlugin();
	}

	std::set< Path > allPaths;

	for( auto ad : allDescs )
	{
        auto paths = GetAllPathsFromAssets( ad, relativeTo );
		allPaths.insert( paths.begin(), paths.end() );
	}

	return PathVec( allPaths.begin(), allPaths.end() );
}

// ********************************
//
PathVec			SceneAccessor::GetAllPathsFromAssets( const AssetDescConstPtr & assetDesc, const Path & relativeTo )
{
	std::set< Path > uniqueAssetsPath;
	if( auto stad = std::dynamic_pointer_cast< SingleTextureAssetDescConstPtr::element_type >( assetDesc ) )
	{
		uniqueAssetsPath.insert( Path::RelativePath( stad->GetImagePath(), relativeTo ) );
	}
	else if( auto tad = std::dynamic_pointer_cast< TextureAssetDescConstPtr::element_type >( assetDesc ) )
	{
		auto stad = tad->GetOrigTextureDesc();
		if( stad )
		{
			uniqueAssetsPath.insert( Path::RelativePath( stad->GetImagePath(), relativeTo ) );
		}

		auto mmad = tad->GetMipMapsDesc();
		if( mmad )
		{
			for( SizeType i = 0; i < mmad->GetLevelsNum(); ++i )
			{
				uniqueAssetsPath.insert( Path::RelativePath( mmad->GetLevelDesc( i )->GetImagePath(), relativeTo ) );
			}
		}
	}
	else
	{
		assert( !"Not implemented" );  // TODO: Implement for the rest of AssetDesc types
		return PathVec();
	}

	return PathVec( uniqueAssetsPath.begin(), uniqueAssetsPath.end() );
}

} // bv