#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "Serialization/XML/XMLDeserializer.h"
#include "Serialization/XML/XMLSerializer.h"

#include "Assets/AssetDescsWithUIDs.h"

#include <fstream>

namespace bv
{

// ********************************
//
SceneDescriptor::SceneDescriptor( const Path & path )
	: m_path( path )
{}

// ********************************
//
Path SceneDescriptor::GetPath() const
{
	return m_path;
}

// ********************************
//
void			            SceneDescriptor::SaveScene		( const model::SceneModelPtr & scene, const Path & outputFilePath )
{
    File::Touch( outputFilePath.Str() );

    auto f = File::Open( outputFilePath.Str(), File::OpenMode::FOMReadWrite );

    SaveScene( scene, *f.StreamBuf() );
    f.Close();
}

// ********************************
//
model::SceneModelPtr	    SceneDescriptor::LoadScene		( const Path & inputFilePath )
{
    auto f = File::Open( inputFilePath.Str() );

    auto size = File::Size( inputFilePath.Str() );

    auto ret = LoadScene( *f.StreamBuf(), size );
    f.Close();

    return ret;
}

namespace 
{
//// *******************************
////
//void GetAssetsWithUIDs( AssetDescsWithUIDs& map, model::BasicNodePtr root )
//{
//    auto plugins = root->GetPlugins();
//    for( unsigned int i = 0; i < root->GetNumPlugins(); i++ )
//    {
//        auto assets = root->GetPlugins()->GetPlugin( i )->GetAssets();
//        for( auto asset : assets )
//            map.AddAssetDesc( asset );
//    }
//
//    for( unsigned int i = 0; i < root->GetNumChildren(); i++ )
//        GetAssetsWithUIDs( map, root->GetChild( i ) );
//}

} // anonymous
// ********************************
//
void			            SceneDescriptor::SaveScene		( const model::SceneModelPtr & scene, std::ostream & out )
{
	auto sob = XMLSerializer();

    sob.EnterChild( "scene" );

    AssetDescsWithUIDs assets;
	GetAssetsWithUIDs( assets, scene->GetRootNode() );
    AssetDescsWithUIDs::SetInstance( assets );

    assets.Serialize( sob );

    scene->Serialize( sob );

    sob.ExitChild();
    sob.Save( out );
}

// ********************************
//
model::SceneModelPtr	SceneDescriptor::LoadScene		( std::istream & in, SizeType numBytes )
{
    auto deser = XMLDeserializer( in, numBytes );

    auto scene = SerializationHelper::DeserializeObjectLoadImpl< model::SceneModel >( deser, "scene" );

    return scene;
}

// ********************************
//
AssetDescVec SceneDescriptor::ListSceneAssets ( const Path & sceneFile )
{
    auto f = File::Open( sceneFile.Str() );

    auto size = File::Size( sceneFile.Str() );

    auto ret = ListSceneAssets( *f.StreamBuf(), size );
    f.Close();

    return ret;
}

// ********************************
//
AssetDescVec SceneDescriptor::ListSceneAssets ( std::istream & in, SizeType numBytes )
{
    auto deser = XMLDeserializer( in, numBytes );

    // assets
    auto assets = SerializationHelper::DeserializeObjectLoadImpl< AssetDescsWithUIDs >( deser, "assets" );

    return assets->GetAssetsDescs();
}

} // bv