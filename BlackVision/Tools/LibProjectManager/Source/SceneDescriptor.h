#pragma once
#include "System/Path.h"

#include "Engine/Editors/BVProject.h"
#include "Assets/FwdDecls.h"

namespace bv
{

class SceneDescriptor
{
private:
	Path				            m_path;

public:

	Path				GetPath			() const;

    static void			            SaveScene		( const model::SceneModelPtr & scene, const Path & outputFilePath );
    static model::SceneModelPtr	    LoadScene		( const Path & inputFilePath );

    static void			            SaveScene		( const model::SceneModelPtr & scene, std::ostream & out );
    static model::SceneModelPtr	    LoadScene		( std::istream & in, SizeType numBytes );

    static AssetDescVec             ListSceneAssets ( const Path & inputFilePath );
    static AssetDescVec             ListSceneAssets ( std::istream & in, SizeType numBytes );

	explicit			            SceneDescriptor	( const Path & path );

};

} // bv