#include "PboUpdatesInspector.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Prototypes/Engine/Common/ScenePrototype.h"


namespace bv {

// *****************************
//
PboUpdatesInspector::PboUpdatesInspector      ( Renderer * renderer )
    : m_renderer( renderer )
    , m_scene( nullptr )
{
}

// *****************************
//
PboUpdatesInspector::~PboUpdatesInspector     ()
{
    delete m_scene;
}

// *****************************
//
void    PboUpdatesInspector::Initialize          ()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );
    glDisable( GL_DEPTH_TEST );

    m_scene = ScenePrototype::CreateNewPrototype( 1, m_renderer );
}

// *****************************
//
void    PboUpdatesInspector::Update              ( TimeType t )
{
    m_scene->Update( t );
}

// *****************************
//
void    PboUpdatesInspector::Render              ()
{
    Render( m_renderer );
}

// *****************************
//
void    PboUpdatesInspector::Key                 ( unsigned char c )
{
    m_scene->OnKey( c );
}

// *****************************
//
void    PboUpdatesInspector::Resize              ( UInt32 w, UInt32 h )
{
    { w; h; }
}

// *****************************
//
void    PboUpdatesInspector::Render              ( Renderer * renderer )
{
    glClear( GL_COLOR_BUFFER_BIT );
    //glClearDepth((GLclampd)m_ClearDepth);

    if( m_scene->GetRootNode() )
    {
        DrawNode( renderer, m_scene->GetRootNode() );
    }
}

// *****************************
//
void    PboUpdatesInspector::DrawNode            ( Renderer * renderer, SceneNode * node )
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
