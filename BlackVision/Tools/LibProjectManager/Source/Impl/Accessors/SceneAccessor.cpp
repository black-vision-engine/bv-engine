#include "SceneAccessor.h"

#include "Engine/Models/Plugins/Plugin.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibBlackVision

#include <set>

namespace bv
{

const std::string SceneAccessor::SceneFileExt = ".*/.scn";

// ********************************
//
SceneAccessorConstPtr SceneAccessor::Create( const Path & path, const model::TimelineManager * tm, const model::PluginsManager * pm )
{
	return SceneAccessorConstPtr( new SceneAccessor( path, tm, pm ) );
}

// ********************************
//
SceneAccessor::SceneAccessor( const Path & path, const model::TimelineManager * tm, const model::PluginsManager * pm )
	: m_rootDir( path )
	, m_tm( tm )
	, m_pm( pm )
{}

// ********************************
//
SceneDescriptor	SceneAccessor::GetSceneDesc( const Path & path ) const
{
	if( Path::Exists( m_rootDir / path ) )
	{
		return SceneDescriptor( m_rootDir / path );
	}
	else
	{
		return SceneDescriptor( "" );
	}
}

// ********************************
//
BVSceneConstPtr	SceneAccessor::GetScene( const Path & path ) const
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
void			SceneAccessor::AddScene( const BVSceneConstPtr & scene, const Path & path ) const
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
	    auto f = File::Open( ( m_rootDir / importToPath ).Str(), File::OpenMode::FOMReadWrite );
        rapidxml::xml_document<> doc;
        std::stringstream buffer;
        buffer << in;
        std::string content( buffer.str() );
        doc.parse<0>( &content[0] );

        auto docNode = doc.first_node( "scene" );

	    f.Write( docNode->value(), docNode->value_size() );

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

		SceneDescriptor sceneDesc( m_rootDir / path );

		auto scene = sceneDesc.LoadScene();

		auto sob = new SerializeObject();

		sob->SetName( "scene" );
		scene->Serialize( *sob );
		sob->Pop();
		sob->Save( out );

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
	return Path::List( m_rootDir / path, SceneFileExt );
}

// ********************************
//
PathVec			SceneAccessor::ListAllUsedAssets( const Path & path ) const
{
	SceneDescriptor desc( path );
	auto scene = desc.LoadScene();


	return GetAllUsedAssetPaths( scene );
}

// ********************************
//
PathVec			SceneAccessor::GetAllUsedAssetPaths( const BVSceneConstPtr & scene )
{
	auto rootNode = scene->GetModelSceneRoot();

	auto lastPlugin = rootNode->GetPlugins()->GetLastPlugin();

	std::vector< AssetDescConstPtr > allDescs;

	while( lastPlugin )
	{
		auto assetsDescs = std::static_pointer_cast< model::BasePlugin< model::IPlugin > >( lastPlugin )->GetAssets();
		allDescs.insert( allDescs.end(), assetsDescs.begin(), assetsDescs.end() );
	}

	std::set< Path > allPaths;

	for( auto ad : allDescs )
	{
		auto paths = GetAllPathsFromAssets( ad );
		allPaths.insert( paths.begin(), paths.end() );
	}

	return PathVec( allPaths.begin(), allPaths.end() );
}

// ********************************
//
PathVec			SceneAccessor::GetAllPathsFromAssets( const AssetDescConstPtr & assetDesc )
{
	std::set< Path > uniqueAssetsPath;
	if( auto stad = std::dynamic_pointer_cast< SingleTextureAssetDescConstPtr::element_type >( assetDesc ) )
	{
		uniqueAssetsPath.insert( stad->GetImagePath() );
	}
	else if( auto tad = std::dynamic_pointer_cast< TextureAssetDescConstPtr::element_type >( assetDesc ) )
	{
		auto stad = tad->GetOrigTextureDesc();
		if( stad )
		{
			uniqueAssetsPath.insert( stad->GetImagePath() );
		}

		auto mmad = tad->GetMipMapsDesc();
		if( mmad )
		{
			for( SizeType i = 0; i < mmad->GetLevelsNum(); ++i )
			{
				uniqueAssetsPath.insert( mmad->GetLevelDesc( i )->GetImagePath() );
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