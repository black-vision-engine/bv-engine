#include "SceneDescriptor.h"

#include "Engine/Models/BasicNode.h"

namespace bv
{

// ********************************
//
BVSceneConstPtr	SceneDescriptor::LoadScene() const
{
    auto f = File::Open( m_path.Str(), File::OpenMode::FOMReadOnly );
    rapidxml::xml_document<> doc;
    std::stringstream buffer;

    f.Read( buffer );

    std::string content( buffer.str() );
    doc.parse<0>( &content[0] );

    auto docNode = doc.first_node( "scene" );

    auto deDoc = DeserializeObject( *docNode, *m_tm, *m_pm);

    ISerializablePtr node = model::BasicNode::Create( deDoc );

    f.Close();

    return std::static_pointer_cast< const BVScene >( node );
}

// ********************************
//
void			SceneDescriptor::SaveScene		( const BVSceneConstPtr & scene, const Path & outPath )
{
    if( !Path::Exists( outPath ) )
    {
        auto f = File::Open( outPath.Str() );
        f.Close();
    }

	auto sob = new SerializeObject();

	sob->SetName( "scene" );
	scene->Serialize( *sob );
	sob->Pop();
	sob->Save( outPath.Str() );
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

} // bv