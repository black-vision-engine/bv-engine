#include "System/Path.h"

#include "Engine/Models/BVScene.h"

namespace bv
{

class SceneDescriptor
{
private:
	Path				m_path;

public:

	Path				GetPath			() const;

	BVSceneConstPtr		LoadScene		() const;

	static void			SaveScene		( const BVSceneConstPtr & scene, const Path & outPath );

	explicit			SceneDescriptor	( const Path & path );

};

} // bv