#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"
#include "Engine/Models/Plugins/Manager/PluginsManager.h"

#include "Serialization/XML/XMLDeserializer.h"
#include "Serialization/BV/XML/BVXMLSerializer.h"
#include "Serialization/BV/BVDeserializeContext.h"

#include "Assets/AssetDescsWithUIDs.h"

#include "Serialization/SerializationHelper.h"

#include "Exceptions/FileNotFoundException.h"

#include "UseLoggerLibBlackVision.h"

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
LoadSceneResult    SceneDescriptor::LoadScene		( const Path & inputFilePath )
{
    auto f = File::Open( inputFilePath.Str() );

    if( f.Good() )
    {
        LOG_MESSAGE( SeverityLevel::info ) << "Loading scene from file: " << inputFilePath;

        auto size = File::Size( inputFilePath.Str() );

        auto ret = LoadScene( *f.StreamBuf(), size );
        f.Close();

        return ret;
    }
    else
    {
        return LoadSceneResult::fromError( std::make_shared< FileNotFoundException >() );
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
    auto context = BVSerializeContext::CreateContextFromEmptiness();
    BVXMLSerializer ser( context );

    scene->Serialize( ser );

    ser.Save( out );
}

// ********************************
//
LoadSceneResult	    SceneDescriptor::LoadScene		( std::istream & in, SizeType numBytes )
{
    try
    {
        auto context = BVDeserializeContext::CreateContextFromEmptiness();
        XMLDeserializer deser( in, numBytes,  context );

        auto scene = SerializationHelper::DeserializeObject< model::SceneModel >( deser, "scene" );
        return LoadSceneResult( scene, context->GetWarnings() );
    }
    catch( const std::exception & e )
    {
        LOG_MESSAGE( SeverityLevel::error ) << "Loading scene failed. Exception: [" << e.what() << "].";
        return e.what();
    }
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
    XMLDeserializer deser( in, numBytes, new BVDeserializeContext( nullptr, nullptr, &model::PluginsManager::DefaultInstanceRef(), model::TimelineManager::GetInstance() ) );

    // assets
    auto assets = SerializationHelper::DeserializeObject< AssetDescsWithUIDs >( deser, "assets" );

    return assets->GetAssetsDescs();
}

} // bv