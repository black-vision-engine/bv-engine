#pragma once
#include "System/Path.h"

#include "Engine/Editors/BVProject.h"
#include "Assets/FwdDecls.h"

#include "LoadSceneResult.h"

namespace bv
{

class SceneDescriptor
{
private:
	Path				            m_path;

public:

	Path				            GetPath			() const;

    static void			            SaveScene		( const model::SceneModelPtr & scene, const Path & outputFilePath );
    static LoadSceneResult          LoadScene		( const Path & inputFilePath );

    static void			            SaveScene		( const model::SceneModelPtr & scene, std::ostream & out );
    static LoadSceneResult	        LoadScene		( std::istream & in, SizeType numBytes );

    static AssetDescVec             ListSceneAssets ( const Path & inputFilePath );
    static AssetDescVec             ListSceneAssets ( std::istream & in, SizeType numBytes );

	explicit			            SceneDescriptor	( const Path & path );

};

} // bv