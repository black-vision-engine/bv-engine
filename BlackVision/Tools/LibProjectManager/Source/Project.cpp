#include "Project.h"

namespace bv
{

// ********************************
//
Project::Project( const Path & name, const Path & rootDir )
	: m_name( name )
	, m_rootDir( rootDir )
{}

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

} // bv
