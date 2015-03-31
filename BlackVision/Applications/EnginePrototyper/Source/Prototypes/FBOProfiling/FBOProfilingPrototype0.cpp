#include "FBOProfilingPrototype0.h"

#include "Engine/Graphics/Renderers/Renderer.h"
#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture2D.h"

namespace bv {

// **************************
//
FBOProfilingPrototype0::FBOProfilingPrototype0    ( Renderer * renderer )
	: m_renderer( renderer )
	, m_rct( 1.f, 1.f )	
	, m_paused( false )
{
	if( !PrepareReadBackBuffers() )
    {
        exit( 1 );
    }

    if( !PrepareShader() )
    {
        exit( 1 );
    }
}

// **************************
//
FBOProfilingPrototype0::~FBOProfilingPrototype0   ()
{
}

// **************************
//
void    FBOProfilingPrototype0::Initialize     ()
{
    BVGL::bvglClearColor( 0.f, 0.f, 0.f, 0.f );
}

// **************************
//
void    FBOProfilingPrototype0::Update         ( TimeType t )
{
    if( m_paused )
        return;

    float r = ( cos( t ) + 1.f ) * 0.5f;
    float g = ( sin( t ) + 1.f ) * 0.5f;
    float b = ( cos( t / 3.f ) * sin( t / 2.f ) + 1.f ) * 0.5f;

    m_prog.SetUniform( "color", glm::vec4( r, g, b, 1.f ) );
}

// **************************
//
void    FBOProfilingPrototype0::Render         ()
{
    BVGL::bvglClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    m_prog.Use();
	m_rct.Render();
}

// **************************
//
void    FBOProfilingPrototype0::Key            ( unsigned char c )
{
    { c; }

    m_paused = !m_paused;
}

// **************************
//
void    FBOProfilingPrototype0::Resize         ( UInt32 w, UInt32 h )
{
    BVGL::bvglViewport( 0, 0, w, h );
}

// **************************
//
bool    FBOProfilingPrototype0::PrepareShader  ()
{
    std::string shadersRoot = config::PROTOTYPES_SHADERS_ROOT + "FBOProfilingPrototype/";

    std::string vsFile = shadersRoot + "basic_color.vert";
    std::string psFile = shadersRoot + "basic_color.frag";

    if( !m_prog.CompileShaderFromFile( vsFile.c_str(), GL_VERTEX_SHADER ) )
    {
        printf( "Vertex shader failed to compile!\n%s", m_prog.Log().c_str());

        return false;
    }

    if( !m_prog.CompileShaderFromFile( psFile.c_str(), GL_FRAGMENT_SHADER ) )
    {
        printf( "Pixel shader failed to compile!\n%s", m_prog.Log().c_str());

        return false;
    }

    if( !m_prog.Link() )
    {
        printf( "Shader program failed to link!\n%s", m_prog.Log().c_str() );

        return false;
    }

    m_prog.Use();

    return true;
}

// **************************
//
bool    FBOProfilingPrototype0::PrepareReadBackBuffers()
{
	BVGL::bvglGenFramebuffers( 1, m_fboID );
    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID[ 0 ] );

	GLint texId = 0;
	BVGL::bvglGetIntegerv( GL_TEXTURE_BINDING_2D, &texId );

	AddColorAttachments();

	BVGL::bvglBindTexture( GL_TEXTURE_2D, texId );
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );

	auto status = BVGL::bvglCheckFramebufferStatus( GL_FRAMEBUFFER );

    if( status != GL_FRAMEBUFFER_COMPLETE )
    {
        printf( "FBO creation failed with status: %d\n", status );

        return false;
    }

    return true;
}

// ****************************
//
void	FBOProfilingPrototype0::AddColorAttachments()
{
	auto tx = std::make_shared< Texture2DImpl >( TextureFormat::F_A8R8G8B8, 1920, 1080, DataBuffer::Semantic::S_TEXTURE_STATIC );
    assert( !m_renderer->IsRegistered( tx.get() ) );

    PdrTexture2D * pdrTx = PdrTexture2D::Create( tx.get() );
    m_renderer->RegisterTexture2D( tx.get(), pdrTx );

    m_texId = pdrTx->GetTextureID();
    m_drawBuff = GL_COLOR_ATTACHMENT0;

    BVGL::bvglBindTexture( GL_TEXTURE_2D, pdrTx->GetTextureID() );
        
    //FIXME: no mipmaps here
    //FIXME: only NEAREST filters used here - should be just fine, but some implementations use linear filtering for some reasons here
    BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    BVGL::bvglFramebufferTexture2D( GL_FRAMEBUFFER, m_drawBuff, GL_TEXTURE_2D, m_texId, 0 );
}


// ****************************
//
void	FBOProfilingPrototype0::EnableFrameBuffer	()
{
	BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, m_fboID[ 0 ] );
    BVGL::bvglDrawBuffers( ( GLsizei )1, &m_drawBuff );

	int      prevViewportCoords[ 4 ] = { 0, 0, 0, 0 };
	double   prevDepthRange[ 2 ] = { 0.0, 0.0 };

    BVGL::bvglGetIntegerv( GL_VIEWPORT, prevViewportCoords );
    BVGL::bvglGetDoublev( GL_DEPTH_RANGE, prevDepthRange );

    BVGL::bvglViewport( 0, 0, 1920, 1080 );
    BVGL::bvglDepthRange( 0.0, 1.0 );
}

// ****************************
//
void	FBOProfilingPrototype0::DisableFrameBuffer	()
{
    BVGL::bvglBindFramebuffer( GL_FRAMEBUFFER, 0 );

    BVGL::bvglViewport( 0, 0, 0, 0 );
    BVGL::bvglDepthRange( 0.0, 0.0 );
}

} // bv
