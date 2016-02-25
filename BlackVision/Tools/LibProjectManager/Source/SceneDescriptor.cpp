#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "Serialization/XML/XMLDeserializer.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Assets/AssetDescsWithUIDs.h"

#include "Serialization/SerializationHelper.h"

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

    if( f.Good() )
    {
        SaveScene( scene, *f.StreamBuf() );
        f.Close();
    }
}

// ********************************
//
model::SceneModelPtr	    SceneDescriptor::LoadScene		( const Path & inputFilePath )
{
    auto f = File::Open( inputFilePath.Str() );

    if( f.Good() )
    {
        auto size = File::Size( inputFilePath.Str() );

        auto ret = LoadScene( *f.StreamBuf(), size );
        f.Close();

        return ret;
    }
    else
    {
        return nullptr;
    }
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
	auto ser = BVXMLSerializer();

    scene->Serialize( ser );

    ser.Save( out );
}

// ********************************
//
model::SceneModelPtr	SceneDescriptor::LoadScene		( std::istream & in, SizeType numBytes )
{
    auto deser = XMLDeserializer( in, numBytes, new BVDeserializeContext( nullptr, nullptr ) );

    auto scene = SerializationHelper::DeserializeObject< model::SceneModel >( deser, "scene" );

    return scene;
}

// ********************************
//
AssetDescVec SceneDescriptor::ListSceneAssets ( const Path & sceneFile )
{
    auto f = File::Open( sceneFile.Str() );

    if( f.Good() )
    {
        auto size = File::Size( sceneFile.Str() );

        auto ret = ListSceneAssets( *f.StreamBuf(), size );
        f.Close();

        return ret;
    }
    else
    {
        return AssetDescVec();
    }
}

// ********************************
//
AssetDescVec SceneDescriptor::ListSceneAssets ( std::istream & in, SizeType numBytes )
{
    auto deser = XMLDeserializer( in, numBytes, new BVDeserializeContext( nullptr, nullptr ) );

    // assets
    auto assets = SerializationHelper::DeserializeObject< AssetDescsWithUIDs >( deser, "assets" );

    return assets->GetAssetsDescs();
}

} // bv