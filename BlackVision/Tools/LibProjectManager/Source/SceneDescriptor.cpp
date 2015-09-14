#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "rapidxml/RapidXml.hpp"
#include "Assets/AssetDescsWithUIDs.h"

namespace bv
{

// ********************************
//
SceneDescriptor::SceneDescriptor( const Path & path, model::TimelineManager * tm )
	: m_path( path )
    , m_tm( tm )
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

// *******************************
//
void GetAssetsWithUIDs( AssetDescsWithUIDs& map, model::BasicNodePtr root )
{
    auto plugins = root->GetPlugins();
    for( unsigned int i = 0; i < root->GetNumPlugins(); i++ )
    {
        auto assets = root->GetPlugins()->GetPlugin( i )->GetAssets();
        for( auto asset : assets )
            map.AddAssetDescWithUID( asset, std::to_string( map.GetNum() ) ); // FIXME: sensible uids would be more sensible
    }

    for( unsigned int i = 0; i < root->GetNumChildren(); i++ )
        GetAssetsWithUIDs( map, root->GetChild( i ) );
}

// ********************************
//
void			            SceneDescriptor::SaveScene		( const model::BasicNodeConstPtr & scene, model::TimelineManager * tm, std::ostream & out )
{
	auto sob = SerializeObject();

    sob.SetName( "scene" );

    AssetDescsWithUIDs assets;
    GetAssetsWithUIDs( assets, std::const_pointer_cast< model::BasicNode >( scene ) );
    AssetDescsWithUIDs::SetInstance( assets );

    assets.Serialize( sob );

    tm->Serialize( sob );
    scene->Serialize( sob );

    sob.Pop();
    sob.Save( out );
}

// ********************************
//
model::BasicNodeConstPtr	SceneDescriptor::LoadScene		( std::istream & in, SizeType numBytes, model::TimelineManager * tm )
{
    //auto docNode = doc.first_node( "scene" );
    auto deDoc = DeserializeObject( in, numBytes, tm );


    // assets
    auto assets = deDoc.Load< AssetDescsWithUIDs >( "assets" );
    AssetDescsWithUIDs::SetInstance( *assets );
    auto timelines = deDoc.LoadArray< model::TimeEvaluatorBase< model::ITimeEvaluator > >( "timelines" );
    for( auto timeline : timelines )
        tm->AddTimeline( timeline );

    auto node = deDoc.Load< model::BasicNode >( "node" );
    assert( node );
    return node;
    //docNode = doc.first_node( "scene" )->first_node( "node" );
    //deDoc = DeserializeObject( docNode, tm );

    //ISerializablePtr node = model::BasicNode::Create( deDoc );

    //return std::static_pointer_cast< const model::BasicNode >( node );
}


} // bv