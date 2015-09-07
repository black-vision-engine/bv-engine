#pragma once
#include "System/Path.h"

#include "Engine/Models/BVScene.h"

namespace bv
{

class SceneDescriptor
{
private:
	Path				            m_path;
	model::TimelineManager *	    m_tm;
	const model::PluginsManager *	m_pm;

public:

	Path				GetPath			() const;

	BVSceneConstPtr		LoadScene		() const;

	static void			SaveScene		( const BVSceneConstPtr & scene, const Path & outPath );

	explicit			SceneDescriptor	( const Path & path, model::TimelineManager * tm, const model::PluginsManager * pm );

};

} // bv