#include "stdafx.h"

#include "PdrSampler.h"

#include "Engine/Graphics/Renderers/Renderer.h"

#include "Engine/Graphics/Shaders/TextureSampler.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"




#include "Memory/MemoryLeaks.h"



namespace bv 
{

// *******************************
//
				PdrSampler::PdrSampler		()
	: m_samplerName( 0 )
{
    BVGL::bvglGenSamplers( 1, &m_samplerName );
}

// *******************************
//
				PdrSampler::PdrSampler		( const TextureSampler * sampler )
	: m_samplerName( 0 )
{
    BVGL::bvglGenSamplers( 1, &m_samplerName );
	SetSamplerParameters( sampler );
}

// *******************************
//
				PdrSampler::~PdrSampler		()
{
	BVGL::bvglDeleteSamplers( 1, &m_samplerName );
}
				
// *******************************
//
void PdrSampler::SetSamplerParameters ( const TextureSampler * sampler )
{
	GLint wrapS = ( GLint ) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_S ) );
	GLint wrapT = ( GLint ) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_T ) );
	GLint wrapR = ( GLint ) ConstantsMapper::GLConstant( sampler->WrappingMode( SamplerWrapDirection::SWD_R ) );

    BVGL::bvglSamplerParameteri( m_samplerName, GL_TEXTURE_WRAP_S, wrapS );
    BVGL::bvglSamplerParameteri( m_samplerName, GL_TEXTURE_WRAP_T, wrapT );
    BVGL::bvglSamplerParameteri( m_samplerName, GL_TEXTURE_WRAP_R, wrapR );

    //FIXME: think a bit more about how filtering mag/min (and mipmaps) should be implemented
    GLint minFilter = ( GLint ) ConstantsMapper::GLConstant( sampler->FilteringMode() );
    GLint magFilter = ( GLint ) ConstantsMapper::GLConstant( sampler->FilteringMode() );

    BVGL::bvglSamplerParameteri( m_samplerName, GL_TEXTURE_MIN_FILTER, minFilter );
    BVGL::bvglSamplerParameteri( m_samplerName, GL_TEXTURE_MAG_FILTER, magFilter );

    BVGL::bvglSamplerParameterfv( m_samplerName, GL_TEXTURE_BORDER_COLOR, &sampler->GetBorderColor()[ 0 ] );
}

// *******************************
//
GLuint			PdrSampler::Bind			( unsigned int textureUnit )
{
	GLint current = 0;
    BVGL::bvglGetIntegerv( GL_SAMPLER_BINDING, &current );

	BVGL::bvglBindSampler( ( GLuint )textureUnit, m_samplerName );

    return ( GLuint ) current;
}

// *******************************
//
void			PdrSampler::Unbind			( unsigned int textureUnit )
{
	BVGL::bvglBindSampler( textureUnit, 0 );
}

// *******************************
//
GLuint          PdrSampler::GetSamplerName    () const
{
	return m_samplerName;
}

} //bv