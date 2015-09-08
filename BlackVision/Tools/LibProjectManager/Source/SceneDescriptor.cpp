#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"
#include "Engine/Models/Timeline/TimelineManager.h"

namespace bv
{
extern model::TimelineManager * global_tm;

// ********************************
//
model::BasicNodeConstPtr SceneDescriptor::LoadScene() const
{
    auto f = File::Open( m_path.Str(), File::OpenMode::FOMReadOnly );
    rapidxml::xml_document<> doc;
    std::stringstream buffer;

    f.Read( buffer );

    std::string content( buffer.str() );
    doc.parse<0>( &content[0] );

    auto docNode = doc.first_node( "scene" );
    auto deDoc = DeserializeObject( *docNode, *m_tm, *m_pm );

    auto timelines = deDoc.LoadArray< model::TimeEvaluatorBase< model::ITimeEvaluator > >( "timelines" );
    for( auto timeline : timelines )
        m_tm->AddTimeline( timeline );

    docNode = doc.first_node( "scene" )->first_node( "node" );
    deDoc = DeserializeObject( *docNode, *m_tm, *m_pm );

    ISerializablePtr node = model::BasicNode::Create( deDoc );

    f.Close();

    return std::static_pointer_cast< const model::BasicNode >( node );
}

// ********************************
//
SceneDescriptor::SceneDescriptor( const Path & path, model::TimelineManager * tm, const model::PluginsManager * pm )
	: m_path( path )
    , m_pm( pm )
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
void			            SceneDescriptor::SaveScene		( const model::BasicNodeConstPtr & scene, const Path & outputFilePath )
{
    if( !Path::Exists( outputFilePath ) )
    {
        auto f = File::Open( outputFilePath.Str() );
        f.Close();
    }

    auto f = File::Open( outputFilePath.Str(), File::OpenMode::FOMReadWrite );

    SaveScene( scene, *f.StreamBuf() );
}

// ********************************
//
model::BasicNodeConstPtr	SceneDescriptor::LoadScene		( const Path & inputFilePath, model::TimelineManager * tm, const model::PluginsManager * pm )
{
    auto f = File::Open( inputFilePath.Str() );

    auto size = File::Size( inputFilePath.Str() );

    return LoadScene( *f.StreamBuf(), size, tm, pm );
}

// ********************************
//
void			            SceneDescriptor::SaveScene		( const model::BasicNodeConstPtr & scene, std::ostream & out )
{
	auto sob = new SerializeObject();

	sob->SetName( "scene" );
    global_tm->Serialize( *sob );
	scene->Serialize( *sob );
	sob->Pop();
	sob->Save( out );
}

// ********************************
//
model::BasicNodeConstPtr	SceneDescriptor::LoadScene		( std::istream & in, SizeType numBytes, model::TimelineManager * tm, const model::PluginsManager * pm )
{
    rapidxml::xml_document<> doc;
    std::stringstream buffer;

    auto buf = new char[ numBytes + 1 ];

    buf[ numBytes ] = '\0';

    in.read( buf, numBytes );

    doc.parse<0>( buf );

    auto docNode = doc.first_node( "scene" );
    auto deDoc = DeserializeObject( *docNode, *tm, *pm );

    auto timelines = deDoc.LoadArray< model::TimeEvaluatorBase< model::ITimeEvaluator > >( "timelines" );
    for( auto timeline : timelines )
        tm->AddTimeline( timeline );

    docNode = doc.first_node( "scene" )->first_node( "node" );
    deDoc = DeserializeObject( *docNode, *tm, *pm );

    ISerializablePtr node = model::BasicNode::Create( deDoc );

    return std::static_pointer_cast< const model::BasicNode >( node );
}


} // bv