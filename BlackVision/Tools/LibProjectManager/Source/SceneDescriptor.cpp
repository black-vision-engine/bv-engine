#include "SceneDescriptor.h"

namespace bv
{

// ********************************
//
BVSceneConstPtr	SceneDescriptor::LoadScene() const
{
	// BVScene::Create( ) cos tam cos tam. Czekam na creeda.
	return nullptr;
}

// ********************************
//
void			SceneDescriptor::SaveScene		( const BVSceneConstPtr & scene, const Path & outPath )
{
	{ scene; }
	{ outPath; }
	// cos tam cos tam. Czekam na creeda
}

// ********************************
//
SceneDescriptor::SceneDescriptor( const Path & path )
	: m_path( path )
{}

// ********************************
//
Path SceneDescriptor::GetPath() const
{
	return m_path;
}

} // bv