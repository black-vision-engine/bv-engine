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

public:

	Path				GetPath			() const;

    static void			            SaveScene		( const model::BasicNodeConstPtr & scene, model::TimelineManager * tm, const Path & outputFilePath );
    static model::BasicNodeConstPtr	LoadScene		( const Path & inputFilePath, model::TimelineManager * tm );

    static void			            SaveScene		( const model::BasicNodeConstPtr & scene, model::TimelineManager * tm, std::ostream & out );
    static model::BasicNodeConstPtr	LoadScene		( std::istream & in, SizeType numBytes, model::TimelineManager * tm );

	explicit			SceneDescriptor	( const Path & path, model::TimelineManager * tm );

};

} // bv