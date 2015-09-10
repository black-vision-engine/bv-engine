#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

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
    rapidxml::xml_document<> doc;
    std::stringstream buffer;

    auto buf = new char[ numBytes + 1 ];

    buf[ numBytes ] = '\0';

    in.read( buf, numBytes );

    doc.parse<0>( buf );

    auto docNode = doc.first_node( "scene" );
    auto deDoc = DeserializeObject( docNode, tm );

    auto timelines = deDoc.LoadArray< model::TimeEvaluatorBase< model::ITimeEvaluator > >( "timelines" );
    for( auto timeline : timelines )
        tm->AddTimeline( timeline );

    docNode = doc.first_node( "scene" )->first_node( "node" );
    deDoc = DeserializeObject( docNode, tm );

    ISerializablePtr node = model::BasicNode::Create( deDoc );

    return std::static_pointer_cast< const model::BasicNode >( node );
}


} // bv