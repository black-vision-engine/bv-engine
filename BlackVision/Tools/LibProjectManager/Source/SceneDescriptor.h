#pragma once
#include "System/Path.h"

#include "Engine/Models/BVScene.h"
#include "Assets/FwdDecls.h"

namespace bv
{

class SceneDescriptor
{
private:
	Path				            m_path;

public:

	Path				GetPath			() const;

    static void			            SaveScene		( const model::BasicNodeConstPtr & scene, model::TimelineManager * tm, const Path & outputFilePath );
    static model::BasicNodeConstPtr	LoadScene		( const Path & inputFilePath, model::TimelineManager * tm );

    static void			            SaveScene		( const model::BasicNodeConstPtr & scene, model::TimelineManager * tm, std::ostream & out );
    static model::BasicNodeConstPtr	LoadScene		( std::istream & in, SizeType numBytes, model::TimelineManager * tm );

    static AssetDescVec             ListSceneAssets ( const Path & inputFilePath );
    static AssetDescVec             ListSceneAssets ( std::istream & in, SizeType numBytes );

	explicit			            SceneDescriptor	( const Path & path );

};

} // bv