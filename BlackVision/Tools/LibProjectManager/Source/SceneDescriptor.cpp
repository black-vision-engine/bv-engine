#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

#include "rapidxml/RapidXml.hpp"

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

// ********************************
//
void			            SceneDescriptor::SaveScene		( const model::BasicNodeConstPtr & scene, model::TimelineManager * tm, std::ostream & out )
{
	auto sob = new SerializeObject();

	sob->SetName( "scene" );
    tm->Serialize( *sob );
	scene->Serialize( *sob );
	sob->Pop();
	sob->Save( out );
}

// ********************************
//
model::BasicNodeConstPtr	SceneDescriptor::LoadScene		( std::istream & in, SizeType numBytes, model::TimelineManager * tm )
{
    //auto docNode = doc.first_node( "scene" );
    auto deDoc = DeserializeObject( in, numBytes, tm );

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