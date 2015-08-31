#include "SceneAccessor.h"

//#include "Engine/Models/Timeline/TimelineManager.h"

namespace bv
{

const std::string SceneAccessor::SceneFileExt = ".*/.scn";

// ********************************
//
SceneAccessor::SceneAccessor( const Path & path, const model::TimelineManager * tm, const model::PluginsManager * pm )
	: m_rootDir( path )
	, m_tm( tm )
	, m_pm( pm )
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
BVSceneConstPtr	SceneAccessor::GetScene( const Path & path ) const
{
	auto desc = GetSceneDesc( path );
	return desc.LoadScene();
}

// ********************************
//
void			SceneAccessor::AddSceneFromFile( const Path & srcPath, const Path & path ) const
{
	Path::Copy( srcPath, path );
}

// ********************************
//
void			SceneAccessor::AddScene( const BVSceneConstPtr & scene, const Path & path ) const
{
	SceneDescriptor::SaveScene( scene, m_rootDir / path );
}

// ********************************
//
void			SceneAccessor::RemoveScene( const Path & path ) const
{
	Path::Remove( m_rootDir / path );
}

// ********************************
//
void			SceneAccessor::ImportScene( std::istream & in, const Path & importToPath ) const
{
	auto f = File::Open( ( m_rootDir / importToPath ).Str(), File::OpenMode::FOMReadWrite );
    rapidxml::xml_document<> doc;
    std::stringstream buffer;
    buffer << in;
    std::string content( buffer.str() );
    doc.parse<0>( &content[0] );

    auto docNode = doc.first_node( "scene" );

	f.Write( docNode->value(), docNode->value_size() );

	f.Close();
}

// ********************************
//
void			SceneAccessor::ExportScene( std::ostream & out, const Path & path ) const
{
	SceneDescriptor sceneDesc( m_rootDir / path );

	auto scene = sceneDesc.LoadScene();

	auto sob = new SerializeObject();

	sob->SetName( "scene" );
	scene->Serialize( *sob );
	sob->Pop();
	sob->Save( out );
}

// ********************************
//
void			SceneAccessor::ImportSceneFromFile( const Path & expFilePath, const Path & importToPath ) const
{
	auto f = File::Open( expFilePath.Str(), File::OpenMode::FOMReadOnly );
	auto in = f.StreamBuf();
	ImportScene( *in, importToPath );
	f.Close();
}

// ********************************
//
void			SceneAccessor::ExportSceneToFile( const Path & outputFileName, const Path & path ) const
{
	auto f = File::Open( outputFileName.Str(), File::OpenMode::FOMReadOnly );
	auto out = f.StreamBuf();
	ExportScene( *out, path );
	f.Close();
}

// ********************************
//
PathVec			SceneAccessor::ListScenes( const Path & path ) const
{
	return Path::List( m_rootDir / path, SceneFileExt );
}

} // bv