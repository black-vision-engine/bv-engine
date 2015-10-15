#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

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
void			            SceneDescriptor::SaveScene		( const model::BasicNodeConstPtr & scene, model::TimelineManager * tm, const Path & outputFilePath )
{
    File::Touch( outputFilePath.Str() );

    auto f = File::Open( outputFilePath.Str(), File::OpenMode::FOMReadWrite );

    SaveScene( scene, tm, *f.StreamBuf() );
    f.Close();
}

// ********************************
//
model::BasicNodeConstPtr	SceneDescriptor::LoadScene		( const Path & inputFilePath, model::TimelineManager * tm )
{
    auto f = File::Open( inputFilePath.Str() );

    auto size = File::Size( inputFilePath.Str() );

    auto ret = LoadScene( *f.StreamBuf(), size, tm );
    f.Close();

    return ret;
}

namespace 
{
// *******************************
//
void GetAssetsWithUIDs( AssetDescsWithUIDs& map, model::BasicNodePtr root )
{
    auto plugins = root->GetPlugins();
    for( unsigned int i = 0; i < root->GetNumPlugins(); i++ )
    {
        auto assets = root->GetPlugins()->GetPlugin( i )->GetAssets();
        for( auto asset : assets )
            map.AddAssetDesc( asset );
    }

    for( unsigned int i = 0; i < root->GetNumChildren(); i++ )
        GetAssetsWithUIDs( map, root->GetChild( i ) );
}

} // anonymous
// ********************************
//
void			            SceneDescriptor::SaveScene		( const model::BasicNodeConstPtr & scene, model::TimelineManager * tm, std::ostream & out )
{
	auto sob = SerializeObject();

    sob.EnterChild( "scene" );

    AssetDescsWithUIDs assets;
    GetAssetsWithUIDs( assets, std::const_pointer_cast< model::BasicNode >( scene ) );
    AssetDescsWithUIDs::SetInstance( assets );

    assets.Serialize( sob );

    tm->Serialize( sob );
    scene->Serialize( sob );

    sob.ExitChild();
    sob.Save( out );
}

// ********************************
//
model::BasicNodeConstPtr	SceneDescriptor::LoadScene		( std::istream & in, SizeType numBytes, model::TimelineManager * tm )
{
    auto deDoc = DeserializeObject( in, numBytes );


    // assets
    auto assets = deDoc.Load< AssetDescsWithUIDs >( "assets" );
    AssetDescsWithUIDs::SetInstance( *assets );

    auto timelines = deDoc.LoadArray< model::TimeEvaluatorBase< model::ITimeEvaluator > >( "timelines" );
    for( auto timeline : timelines )
        for( auto child : timeline->GetChildren() )
            tm->AddTimeline( child );


    auto node = deDoc.Load< model::BasicNode >( "node" );
    assert( node );
    return node;
    //docNode = doc.first_node( "scene" )->first_node( "node" );
    //deDoc = DeserializeObject( docNode, tm );

    //ISerializablePtr node = model::BasicNode::Create( deDoc );

    //return std::static_pointer_cast< const model::BasicNode >( node );
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
    auto deDoc = DeserializeObject( in, numBytes );

    // assets
    auto assets = deDoc.Load< AssetDescsWithUIDs >( "assets" );

    return assets->GetAssetsDescs();
}

} // bv