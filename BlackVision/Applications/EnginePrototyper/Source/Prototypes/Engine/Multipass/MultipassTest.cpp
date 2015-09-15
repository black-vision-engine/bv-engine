#include "MultipassTest.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Prototypes/Engine/Common/ScenePrototype.h"


namespace bv {

// *****************************
//
MultipassTest::MultipassTest      ( Renderer * renderer )
    : m_renderer( renderer )
    , m_scene( nullptr )
{
}

// *****************************
//
MultipassTest::~MultipassTest     ()
{
    delete m_scene;
}

// *****************************
//
void    MultipassTest::Initialize          ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
    BVGL::bvglDisable( GL_DEPTH_TEST );

    m_scene = ScenePrototype::CreateNewPrototype( 3, m_renderer );
}

// *****************************
//
void    MultipassTest::Update              ( TimeType t )
{
    m_scene->Update( t );
}

// *****************************
//
void    MultipassTest::Render              ()
{
    Render( m_renderer );
}

// *****************************
//
void    MultipassTest::Key                 ( unsigned char c )
{
    m_scene->OnKey( c );
}

// *****************************
//
void    MultipassTest::Resize              ( UInt32 w, UInt32 h )
{
    { w; h; }
}

// *****************************
//
void    MultipassTest::Render              ( Renderer * renderer )
{
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT );
    //BVGL::bvglClearDepth((GLclampd)m_ClearDepth);

    if( m_scene->GetRootNode() )
    {
        DrawNode( renderer, m_scene->GetRootNode() );
    }
}

// *****************************
//
void    MultipassTest::DrawNode            ( Renderer * renderer, SceneNode * node )
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

// **************************
//
IBasicLogicUnqPtr    MultipassTest::Create  ( Renderer * renderer )
{
	return IBasicLogicUnqPtr( new MultipassTest( renderer ) );
}

} // bv
