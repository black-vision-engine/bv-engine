#include "stdafx.h"

#include "PdrTexture2D.h"

#include <cassert>

#include "Engine/Graphics/Renderers/OGLRenderer/PdrTexture.h"
//#define POOR_PROFILE_TEXTURE_STREAMING


#ifdef POOR_PROFILE_TEXTURE_STREAMING
#include "Tools/HRTimer.h"
#endif



#include "Memory/MemoryLeaks.h"



namespace bv
{

#ifdef POOR_PROFILE_TEXTURE_STREAMING
extern HighResolutionTimer GTimer;
#endif

// *******************************
// FIXME: implement streaming via two PBOs to make prebuffering "blazingly" fast
// FIXME: implement mipmapping machinery
PdrTexture2D::PdrTexture2D                      ( const Texture2D * texture )
	: m_width( 0 )
    , m_height( 0 )
{
    Update( texture );
}

// *******************************
//
void    PdrTexture2D::Initialize      ( const Texture2D * texture )
{
    assert( texture );

    m_txFormat  = texture->GetFormat();

    m_width     = texture->GetWidth();
    m_height    = texture->GetHeight();


    //FIXME: allow more texture types here
    assert( m_txFormat == TextureFormat::F_A8R8G8B8 ||
			m_txFormat == TextureFormat::F_R8G8B8 ||
			m_txFormat == TextureFormat::F_A8 || 
			m_txFormat == TextureFormat::F_A32FR32FG32FB32F );

    m_internalFormat    = ConstantsMapper::GLConstantTextureInternalFormat( m_txFormat );
    m_format            = ConstantsMapper::GLConstantTextureFormat( m_txFormat );
    m_type              = ConstantsMapper::GLConstantTextureType( m_txFormat );

	auto levels			= texture->GetNumLevels();

    auto txSemantic     = texture->GetSemantic();
    if( PdrPBOMemTransfer::PBORequired( txSemantic ) )
    {
		m_pboMem.reserve( levels );
		for( unsigned int lvl = 0; lvl < levels; ++lvl )
		{
			m_pboMem.push_back( std::unique_ptr< PdrUploadPBO >( new PdrUploadPBO( txSemantic, texture->RawFrameSize( lvl ), false ) ) );
		}
    }


    BVGL::bvglGenTextures   ( 1, &m_textureID );
    GLuint prevTex = Bind();

	BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	BVGL::bvglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, ( GLint )levels );
	
	BVGL::bvglTexStorage2D( GL_TEXTURE_2D, ( GLint )levels, m_internalFormat, ( GLsizei )m_width, ( GLsizei )m_height );
	
	if( m_pboMem.empty() )
	{
		for (unsigned int lvl = 0; lvl < levels; ++lvl)
		{
			auto data = texture->GetData( lvl );
			if( data )
			{
				BVGL::bvglTexSubImage2D( GL_TEXTURE_2D, lvl, 0, 0, 
					( GLsizei )texture->GetWidth( lvl ), ( GLsizei )texture->GetHeight( lvl ), 
					m_format, m_type, data->Get() );
			}
		}
	}

    BVGL::bvglBindTexture( GL_TEXTURE_2D, prevTex );
}

// *******************************
//
void    PdrTexture2D::Deinitialize    ()
{
    if( m_textureID != 0 )
    {
        BVGL::bvglDeleteTextures( 1, &m_textureID );
    }

	m_pboMem.clear();
}

// *******************************
//
void    PdrTexture2D::UpdateTexData     ( const Texture2D * texture )
{
    assert( !m_pboMem.empty() );

#ifdef POOR_PROFILE_TEXTURE_STREAMING
    double writeStart = GTimer.CurElapsed();
#endif

	for( unsigned int lvl = 0; lvl < texture->GetNumLevels(); ++lvl )
	{
		m_pboMem[ lvl ]->LockUpload( texture->GetData( lvl )->Get(), texture->RawFrameSize( lvl ) );
		PBOUploadData( texture, lvl );
		m_pboMem[ lvl ]->UnlockUpload();
	}

#ifdef POOR_PROFILE_TEXTURE_STREAMING
    double writeTime = GTimer.CurElapsed() - writeStart;
    printf( "Frame streaming took %.4f ms\n", writeTime * 1000.f );
#endif
}

// *******************************
//
void    PdrTexture2D::PBOUploadData     ( const Texture2D * texture, UInt32 lvl )
{
	GLint prevTex = Bind();
	BVGL::bvglTexSubImage2D( GL_TEXTURE_2D, lvl, 0, 0, 
		( GLsizei )texture->GetWidth( lvl ), ( GLsizei )texture->GetHeight( lvl ), 
		m_format, m_type, 0 );
	BVGL::bvglBindTexture( GL_TEXTURE_2D, prevTex );
}

// *******************************
//
PdrTexture2D::~PdrTexture2D         ()
{
    Deinitialize();
}

// *******************************
//
void        PdrTexture2D::Update            ( const Texture2D * texture )
{
    if ( texture->GetFormat() != m_txFormat || m_width != texture->GetWidth() || m_height != texture->GetHeight() )
    {
        Deinitialize();
        Initialize( texture );
    }

	if( !m_pboMem.empty() )
    {
        UpdateTexData( texture );
    }
}

// *******************************
//
GLuint      PdrTexture2D::Bind             ()
{
    GLint current = 0;

    BVGL::bvglGetIntegerv( GL_TEXTURE_BINDING_2D, &current ); // FIXME: Optimization. Removing this line make rendering 2 times faster. V, check please if it can be done differently. 
    BVGL::bvglBindTexture( GL_TEXTURE_2D, m_textureID );

    return (GLuint) current;
}

// *******************************
//
void        PdrTexture2D::Unbind            ()
{
    BVGL::bvglBindTexture   ( GL_TEXTURE_2D, m_prevTextureID );     
}

// *******************************
//
PdrTexture2D *  PdrTexture2D::Create            ( const Texture2D * texture )
{
    return new PdrTexture2D( texture );
}

// *******************************
//
void			PdrTexture2D::GenerateMipmaps	()
{
	GLint prevTex = Bind();

	BVGL::bvglGenerateMipmap( GL_TEXTURE_2D );
	BVGL::bvglBindTexture( GL_TEXTURE_2D, prevTex );
}

} // bv
