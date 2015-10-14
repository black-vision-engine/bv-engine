#include "Project.h"

#include "IO/DirIO.h"
#include "IO/FileIO.h"

#include <sstream>
#include <cassert>

namespace bv
{

// ********************************
//
Project::Project( const Path & name, const Path & rootDir )
	: m_name( name )
	, m_rootDir( rootDir / name )
{
	Init();
}

// ********************************
//
ProjectConstPtr		Project::Create		( const Path & name, const Path & rootDir )
{
	return ProjectConstPtr( new Project( name, rootDir ) );
}

// ********************************
//
Path				Project::GetName	() const
{
	return m_name;
}

// ********************************
//
void				Project::Init		() const
{
	if( !Path::Exists( m_rootDir ) )
	{
        if( !Dir::CreateDir( m_rootDir.Str(), true ) )
        {
            return;
        }
	}

	if( !Path::Exists( m_rootDir / ".bvproj" ) )
	{
		auto f = File::Open( ( m_rootDir / ".bvproj" ).Str(), File::OpenMode::FOMReadWrite );

		f.Write( m_name.Str() );
		f.Close();
	}
	else
	{
		std::stringstream str;
		File::Read( str, ( m_rootDir / ".bvproj" ).Str() );

		assert( str.str() == m_name.Str() );
	}
}

} // bv
