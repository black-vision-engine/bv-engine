#pragma once

#include "ProjectManager.h"
#include "SceneDescriptor.h"
#include "System/Path.h"

namespace bv
{

namespace model
{
	class TimelineManager;
	class PluginsManager;
} // model

class SceneAccessor
{
private:

	static const std::string SceneFileExt;

	Path							m_rootDir;
	const model::TimelineManager *	m_tm;
	const model::PluginsManager *	m_pm;

	explicit			SceneAccessor			( const Path & path, const model::TimelineManager * tm, const model::PluginsManager * pm );

public:

	SceneDescriptor		GetSceneDesc			( const Path & path ) const;
	BVSceneConstPtr		GetScene				( const Path & path ) const;

	void				AddSceneFromFile		( const Path & srcPath, const Path & path ) const;
	void				AddScene				( const BVSceneConstPtr & scene, const Path & path ) const;
	void				RemoveScene				( const Path & path ) const;

	void				ImportScene				( std::istream & in, const Path & importToPath ) const;
	void				ExportScene				( std::ostream & out, const Path & path ) const;

	void				ImportSceneFromFile		( const Path & expFilePath, const Path & importToPath ) const;
	void				ExportSceneToFile		( const Path & outputFileName, const Path & path ) const;

	PathVec				ListScenes				( const Path & path ) const;


};

} // bv