#pragma once

#include "CoreDEF.h"

#include "System/Path.h"

namespace bv
{

class Project;
DEFINE_CONST_PTR_TYPE( Project )

class Project
{
public:
	Path					GetName	() const;

	static ProjectConstPtr	Create	( const Path & name, const Path & rootDir );

private:
	explicit				Project	( const Path & name, const Path & rootDir );
	void					Init	() const;


	Path				m_name;
	Path				m_rootDir;
};


} // bv