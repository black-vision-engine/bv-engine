#include "MemManagementInspector.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/SceneGraph/RenderableEntity.h"

#include "Prototypes/Engine/Common/SceneExampleBuilder.h"


namespace bv {

// *****************************
//
MemManagementInspector::MemManagementInspector      ( Renderer * renderer )
    : m_renderer( renderer )
    , m_sceneRoot( nullptr )
{
}

// *****************************
//
MemManagementInspector::~MemManagementInspector     ()
{
    delete m_sceneRoot;
}

// *****************************
//
void    MemManagementInspector::Initialize          ()
{
    glClearColor( 0.f, 0.f, 0.f, 0.f );
    glDisable( GL_DEPTH_TEST );

    m_sceneRoot = SceneExampleBuilder::BuildScene( 0 );
}

// *****************************
//
void    MemManagementInspector::Update              ( TimeType t )
{
    SceneExampleBuilder::UpdateScene( 0, m_sceneRoot, t );
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
    { c; }
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
    glClear( GL_COLOR_BUFFER_BIT );
    //glClearDepth((GLclampd)m_ClearDepth);

    if( m_sceneRoot )
    {
        DrawNode( renderer, m_sceneRoot );
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
