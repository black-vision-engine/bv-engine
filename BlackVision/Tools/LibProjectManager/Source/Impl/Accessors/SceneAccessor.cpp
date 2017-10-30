#include "SceneAccessor.h"

#include "AssetAccessor.h"

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultTextureDescriptor.h"
#include "Engine/Models/Plugins/Channels/PixelShader/DefaultAnimationDescriptor.h"

#include "Assets/FwdDecls.h"

#include "Tools/Logger/Logger.h"
#define LOG_MODULE ModuleEnum::ME_LibBlackVision

#include <set>

#include "rapidxml/RapidXml.hpp"


#include "Serialization/Json/JsonDeserializeObject.h"
#include "Assets/Thumbnail/Impl/SceneThumbnail.h"

#include <fstream>

namespace bv
{

const std::string SceneAccessor::SceneFileExt = ".*\\.scn";

// ********************************
//
SceneAccessorConstPtr SceneAccessor::Create( const Path & path )
{
    return SceneAccessorConstPtr( new SceneAccessor( path ) );
}

// ********************************
//
SceneAccessor::SceneAccessor( const Path & path )
    : m_rootDirPM( path )
    , m_rootDir( path / "scenes" )
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
LoadSceneResult 	SceneAccessor::GetScene( const Path & path ) const
{
    return SceneDescriptor::LoadScene( m_rootDir / path );
}

// ********************************
//
void			SceneAccessor::AddSceneFromFile( const Path & srcPath, const Path & path ) const
{
    Path::Copy( srcPath, path );
}

// ********************************
//
void			SceneAccessor::AddScene( const model::SceneModelPtr & scene, const Path & path ) const
{
    SceneDescriptor::SaveScene( scene, m_rootDir / path );
}

// ********************************
//
void			SceneAccessor::RemoveScene( const Path & path ) const
{
    Path::Remove( m_rootDir / path );
}

// ***********************
//
ThumbnailConstPtr       SceneAccessor::GetSceneThumbnail   ( const Path & path ) const
{
    auto absolutePath = m_rootDir / path;
    Path thumbPath = AssetAccessor::GetThumbnailPath( absolutePath );

    if( !Path::Exists( thumbPath ) )
        return nullptr;

    JsonDeserializeObject deser;
    deser.LoadFile( thumbPath.Str() );

    auto thumb = SceneThumbnail::Create( deser );
    return thumb;
}

// ********************************
//
void			SceneAccessor::ImportScene( std::istream & in, const Path & importToProject, const Path & importToPath ) const
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

        buf.str("");
        in.get( buf, '\n' );
        in.ignore();
        Path oldOwnerProjectName = buf.str();

        buf.str("");
        in.get( buf, '\n' );
        auto size = stoul( buf.str() );
        in.ignore();

        auto loadResult = SceneDescriptor::LoadScene( in, size );

        if( loadResult.IsValid() )
        {
            auto scene = loadResult.GetVal().GetScene();
            auto sceneAssertDescs = ListSceneAssetsDescs( scene->GetRootNode() );
        
            std::set< AssetDescConstPtr > assetsDescsSet;
            assetsDescsSet.insert( sceneAssertDescs.begin(), sceneAssertDescs.end() );

            for( auto ad : assetsDescsSet )
            {
                ReplaceRootDir( ad, oldPMRootDir, m_rootDirPM );
                ReplaceProjectName( ad, oldOwnerProjectName, importToProject );
            }

            auto outFilePath = ( m_rootDir / importToProject / importToPath ).Str();

            SceneDescriptor::SaveScene( scene, outFilePath );
        }
        else
            LOG_MESSAGE( SeverityLevel::error ) << "Cannot import scene: " << importToPath.Str(); 

        buf.str( "" );
        in.ignore();
        in.get( buf, '\n' );

        if( buf.str() != "serialized_scene_end" )
        {
            LOG_MESSAGE_FILE_LINE( SeverityLevel::error ) << "Cannot import scene: '" << importToPath.Str() << "'. Wrong format.";
        }
    }
}

// ********************************
//
void			SceneAccessor::ExportScene( std::ostream & out, const Path & projectName, const Path & path ) const
{
    out << "serialized_scene_begin" << '\n';

    out << m_rootDirPM << '\n';

    auto sceneFileName = ( m_rootDir/ projectName / path ).Str();

    out << projectName << '\n';

    out << std::to_string( File::Size( sceneFileName ) ) << '\n';    

    File::Read( out, sceneFileName );

    out << '\n';

    out << "serialized_scene_end" << '\n';
}

// ********************************
//
void			SceneAccessor::ImportSceneFromFile( const Path & expFilePath, const Path & importToProject, const Path & importToPath ) const
{
    auto f = File::Open( expFilePath.Str(), File::OpenMode::FOMReadOnly );
    auto in = f.StreamBuf();
    ImportScene( *in, importToProject, importToPath );
    f.Close();
}

// ********************************
//
void			SceneAccessor::ExportSceneToFile( const Path & projectName, const Path & outputFileName, const Path & path ) const
{
    auto f = File::Open( outputFileName.Str(), File::OpenMode::FOMReadOnly );
    auto out = f.StreamBuf();
    ExportScene( *out, projectName, path );
    f.Close();
}

// ********************************
//
PathVec			SceneAccessor::ListScenes( const Path & path, bool recursive ) const
{
    auto sceneList = Path::List( m_rootDir / path, recursive, SceneFileExt );

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
    auto simpleAssets = ListSceneAssetsDescs( m_rootDir / path );

    std::set< Path > allPaths;

    for( auto ad : simpleAssets )
    {
        auto paths = GetAllPathsFromAssets( ad );
        allPaths.insert( paths.begin(), paths.end() );
    }

    return PathVec( allPaths.begin(), allPaths.end() );
}

// ********************************
//
//PathVec			SceneAccessor::GetAllUsedAssetPaths( const model::BasicNodeConstPtr & scene, const Path & relativeTo )
//{
//	auto rootNode = std::const_pointer_cast< model::BasicNode >( scene );
//
//    auto sceneSimpleAssetsDescs = ListSceneAssetsDescs( scene );
//
//    std::set< Path > assetsPathsSet;
//
//    for( auto a : sceneSimpleAssetsDescs )
//    {
//        assetsPathsSet.insert( AssetDescToPath( a ) );
//    }
//
//    auto lastPlugin = rootNode->GetPlugins()->GetLastPlugin();
//
//	std::vector< AssetDescConstPtr > allDescs;
//
//	while( lastPlugin )
//	{
//		auto assetsDescs = std::static_pointer_cast< model::BasePlugin< model::IPlugin > >( lastPlugin )->GetAssets();
//		allDescs.insert( allDescs.end(), assetsDescs.begin(), assetsDescs.end() );
//        lastPlugin = lastPlugin->GetPrevPlugin();
//	}
//
//	std::set< Path > allPaths;
//
//	for( auto ad : allDescs )
//	{
//        auto paths = GetAllPathsFromAssets( ad, relativeTo );
//		allPaths.insert( paths.begin(), paths.end() );
//	}
//
//	return PathVec( allPaths.begin(), allPaths.end() );
//}

// ********************************
//
AssetDescVec SceneAccessor::UnpackSimpleAssets( const AssetDescConstPtr & assetDesc )
{
    std::vector< AssetDescConstPtr > simpleAssets;
    if( std::dynamic_pointer_cast< SingleTextureAssetDescConstPtr::element_type >( assetDesc ) )
    {
        simpleAssets.push_back( std::dynamic_pointer_cast< SingleTextureAssetDescConstPtr::element_type >( assetDesc ) );
    }
    else if( std::dynamic_pointer_cast< TextureAssetDescConstPtr::element_type >( assetDesc ) )
    {
        auto tad = std::dynamic_pointer_cast< TextureAssetDescConstPtr::element_type >( assetDesc );

        auto stad = tad->GetOrigTextureDesc();
        if( stad )
        {
            simpleAssets.push_back( stad );
        }

        auto mmad = tad->GetMipMapsDesc();
        if( mmad )
        {
            for( SizeType i = 0; i < mmad->GetLevelsNum(); ++i )
            {
                simpleAssets.push_back( mmad->GetLevelDesc( i ) );
            }
        }
    }
    else
    {
        assert( !"Not implemented" );  // TODO: Implement for the rest of AssetDesc types
        return std::vector< AssetDescConstPtr >();
    }

    return simpleAssets;
}

// ********************************
//
AssetDescVec SceneAccessor::ListSceneAssetsDescs( const Path & sceneFile )
{
    auto assets = SceneDescriptor::ListSceneAssets( sceneFile );

    std::vector< AssetDescConstPtr > simpleAssetsDescs;

    for( auto ad : assets )
    {
        auto sa = UnpackSimpleAssets( ad );
        simpleAssetsDescs.insert( simpleAssetsDescs. end(), sa.begin(), sa.end() );
    }

    return simpleAssetsDescs;
}

// ********************************
//
AssetDescVec SceneAccessor::ListSceneAssetsDescs( std::istream & in, SizeType numBytes  )
{
    auto assets = SceneDescriptor::ListSceneAssets( in, numBytes );

    std::vector< AssetDescConstPtr > simpleAssetsDescs;

    for( auto ad : assets )
    {
        auto sa = UnpackSimpleAssets( ad );
        simpleAssetsDescs.insert( simpleAssetsDescs. end(), sa.begin(), sa.end() );
    }

    return simpleAssetsDescs;
}

// ********************************
//
AssetDescVec SceneAccessor::ListSceneAssetsDescs( const model::BasicNodeConstPtr & scene )
{
    auto rootNode = std::const_pointer_cast< model::BasicNode >( scene );

    auto lastPlugin = rootNode->GetPlugins()->GetLastPlugin();

    std::vector< AssetDescConstPtr > allDescs;

    while( lastPlugin )
    {
        auto lassets = std::static_pointer_cast< model::BasePlugin >( lastPlugin )->GetLAssets();
        
        for( auto lasset : lassets )
            allDescs.push_back( lasset.assetDesc );

        lastPlugin = lastPlugin->GetPrevPlugin();
    }

    std::vector< AssetDescConstPtr > simpleAssetsDescs;

    for( auto ad : allDescs )
    {
        auto sa = UnpackSimpleAssets( ad );
        //simpleAssetsDescs.insert( simpleAssetsDescs. end(), sa.begin(), sa.end() );
    }

    return simpleAssetsDescs;
}

// ********************************
//
PathVec			SceneAccessor::GetAllPathsFromAssets( const AssetDescConstPtr & assetDesc )
{
    auto simpleAssetsDescs = UnpackSimpleAssets( assetDesc );

    std::set< Path > uniqueAssetsPath;

    for( auto a : simpleAssetsDescs )
    {
        uniqueAssetsPath.insert( AssetDescToPath( a ) );
    }

    return PathVec( uniqueAssetsPath.begin(), uniqueAssetsPath.end() );
}

// ********************************
//
Path         SceneAccessor::AssetDescToPath( const AssetDescConstPtr & desc )
{
    if( auto typedDesc = std::dynamic_pointer_cast< const SingleTextureAssetDesc >( desc ) )
    {
        return Path( typedDesc->GetImagePath() );
    }
    else
    {
        assert( !"Not implemented" );  // TODO: Implement for the rest of AssetDesc types
        return Path();
    }
}

// ********************************
//
void         SceneAccessor::ReplaceRootDir( const AssetDescConstPtr & ad, const Path & oldPMRootDir, const Path & newPMRootDir )
{
    auto p = AssetDescToPath( ad );

    auto pstr = p.Str();

    auto pos = pstr.find( oldPMRootDir.Str() );

    if( pos != std::string::npos )
    {
        if( pos == 0 )
        {
            auto oldPMRootDirSize = oldPMRootDir.Str().size();
            auto oldRelPath = pstr.substr( oldPMRootDirSize, std::string::npos );

            Path newAssetPath = newPMRootDir / oldRelPath;   

            ReplacePathInSimpleAsset( ad, newAssetPath );
        }
        else
        {
            assert( false ); // FIXME: What should be done in such a situation?
        }
    }
}

// ********************************
//
void         SceneAccessor::ReplaceProjectName( const AssetDescConstPtr & ad, const Path & oldProjectName, const Path & newProjectName )
{
    auto p = AssetDescToPath( ad );

    auto pstr = p.Str();

    auto pos = pstr.find( oldProjectName.Str() );

    if( pos != std::string::npos )
    {
        auto oldProjectNameSize = oldProjectName.Str().size();
        auto oldRelPath = pstr.substr( pos + oldProjectNameSize, std::string::npos );
        auto prefix = pstr.substr( 0, pos );

        Path newAssetPath = Path( prefix ) / newProjectName / oldRelPath;   

        ReplacePathInSimpleAsset( ad, newAssetPath );
    }
}

// ********************************
//
void         SceneAccessor::ReplacePathInSimpleAsset( const AssetDescConstPtr & ad, const Path & path )
{
    if( auto adTyped = std::dynamic_pointer_cast< const SingleTextureAssetDesc >( ad ) )
    {
        std::const_pointer_cast< SingleTextureAssetDesc >( adTyped )->m_imagePath = path.Str();  // FIXME: Very ugly hack.
    }
    else
    {
        assert( false );  // TODO: Implement.
    }
}

} // bv