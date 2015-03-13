#include "MemManagementInspector.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Prototypes/Engine/Common/ScenePrototype.h"

#include "Assets/Texture/TextureAssetDescriptor.h"
#include "Assets/Texture/TextureLoader.h"
#include "Assets/AssetManager.h"


namespace bv {

// *****************************
//
MemManagementInspector::MemManagementInspector      ( Renderer * renderer )
    : m_renderer( renderer )
    , m_scene( nullptr )
{
}

// *****************************
//
MemManagementInspector::~MemManagementInspector     ()
{
    delete m_scene;
}

// *****************************
//
void    MemManagementInspector::Initialize          ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
    BVGL::bvglDisable( GL_DEPTH_TEST );

	AssetManager::GetInstance().RegisterLoader( TextureAssetDesc::UID(), new TextureLoader() );
	// bv::AssetManager::GetInstance().RegisterLoader( bv::FontAssetDesc::UID(), new bv::model::FontLoader() );
	// bv::AssetManager::GetInstance().RegisterLoader( bv::model::AnimationAssetDesc::UID(), new bv::model::AnimationLoader() );

    m_scene = ScenePrototype::CreateNewPrototype( 0, m_renderer );
}

// *****************************
//
void    MemManagementInspector::Update              ( TimeType t )
{
    m_scene->Update( t );
}

// *****************************
//
void    MemManagementInspector::Render              ()
{
    Render( m_renderer );
}

// *****************************
//
void    MemManagementInspector::Key                 ( unsigned char c )
{
    m_scene->OnKey( c );
}

// *****************************
//
void    MemManagementInspector::Resize              ( UInt32 w, UInt32 h )
{
    { w; h; }
}

// *****************************
//
void    MemManagementInspector::Render              ( Renderer * renderer )
{
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT );
    //glClearDepth((GLclampd)m_ClearDepth);

    if( m_scene->GetRootNode() )
    {
        DrawNode( renderer, m_scene->GetRootNode() );
    }
}

// *****************************
//
void    MemManagementInspector::DrawNode            ( Renderer * renderer, SceneNode * node )
{
    auto renderable = static_cast<bv::RenderableEntity *>( node->GetTransformable() );

    if ( renderable )
    {
        renderer->Draw( renderable );

        for ( unsigned int i = 0; i < (unsigned int) node->NumChildNodes(); i++ )
        {
            DrawNode( renderer, node->GetChild( i ) );
        }
    }
}

} // bv
