#include "PdrPBOMemTransfer.h"


namespace bv {

// ****************************
//
					PdrUploadPBO::PdrUploadPBO			( DataBuffer::Semantic semantic, SizeType dataSize )
			: PdrPBOMemTransfer( semantic, dataSize )
{
	assert( m_pboTarget == GL_PIXEL_UNPACK_BUFFER );
}

// ****************************
//
void				PdrUploadPBO::LockUpload			( const char * source, SizeType dataSize )
{
	BVGL::bvglBindBuffer( m_pboTarget, m_pboID[ m_index ] );

	auto data = BVGL::bvglMapBuffer( m_pboTarget, GL_WRITE_ONLY );
	memcpy( data, source, dataSize );
	BVGL::bvglUnmapBuffer( m_pboTarget );

	if( m_numPBOs > 1 )
	{
		m_index = ( m_index + 1 ) % m_numPBOs;
		BVGL::bvglBindBuffer( m_pboTarget, m_pboID[ m_index ] );
	}
}

// ****************************
//
void				PdrUploadPBO::UnlockUpload			()
{
	BVGL::bvglBindBuffer( m_pboTarget, 0 );
}

// ****************************
//
					PdrDownloadPBO::PdrDownloadPBO		( DataBuffer::Semantic semantic, SizeType dataSize )
			: PdrPBOMemTransfer( semantic, dataSize )
{
	assert( m_pboTarget == GL_PIXEL_PACK_BUFFER );
}

// ****************************
//
void				PdrDownloadPBO::LockDownload		()
{
    BVGL::bvglBindBuffer( m_pboTarget, m_pboID[ m_index ] );
}

// ****************************
//
void				PdrDownloadPBO::UnlockDownload		( char * dest, SizeType dataSize )
{
	if( m_numPBOs > 1 )
	{
		m_index = ( m_index + 1 ) % m_numPBOs;
		BVGL::bvglBindBuffer( m_pboTarget, m_pboID[ m_index ] );
	}

	auto data = BVGL::bvglMapBuffer( m_pboTarget, GL_READ_ONLY );
	memcpy( dest, data, dataSize );
	BVGL::bvglUnmapBuffer( m_pboTarget );
		
	BVGL::bvglBindBuffer( m_pboTarget, 0 );
}


// ****************************
//
				PdrPBOMemTransfer::PdrPBOMemTransfer    ( DataBuffer::Semantic semantic, SizeType dataSize )
    : m_index( 0 ) 
    , m_dataSize( ( GLenum )dataSize )
{
    assert( (int) DataBuffer::Semantic::S_TOTAL == 7 );
    assert( PBORequired( semantic ) );

    m_pboTarget = PBOTarget( semantic );
    m_pboUsage  = PBOUsage( semantic );
    m_numPBOs   = NumPBOs( semantic );

    assert( m_numPBOs > 0 );

	m_pboID.resize( m_numPBOs );
    BVGL::bvglGenBuffers( m_numPBOs, &m_pboID[ 0 ] );

    for( unsigned int i = 0; i < m_numPBOs; ++i )
    {
        BVGL::bvglBindBuffer( m_pboTarget, m_pboID[ i ] );
        BVGL::bvglBufferData( m_pboTarget, dataSize, 0, m_pboUsage );
    }
    BVGL::bvglBindBuffer( m_pboTarget, 0 );
}

// ****************************
//
PdrPBOMemTransfer::~PdrPBOMemTransfer   ()
{
    BVGL::bvglDeleteBuffers( m_numPBOs, &m_pboID[ 0 ] );
}

// ****************************
//
bool PdrPBOMemTransfer::PBORequired     ( DataBuffer::Semantic semantic )
{
    return semantic != DataBuffer::Semantic::S_STATIC && semantic != DataBuffer::Semantic::S_TEXTURE_STATIC;
}

// ****************************
//
GLuint  PdrPBOMemTransfer::NumPBOs      ( DataBuffer::Semantic semantic ) const
{
#ifdef DISALLOW_ASYNC_PBO
    return 1
#endif

    if( semantic == DataBuffer::Semantic::S_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_DYNAMIC )
    {
        return 1;
    }
    else if ( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return 2;
    }

    return 0;
}

// ****************************
//
GLenum  PdrPBOMemTransfer::PBOTarget    ( DataBuffer::Semantic semantic ) const
{
    if( semantic == DataBuffer::Semantic::S_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return GL_PIXEL_UNPACK_BUFFER;
    }
    else if ( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ )
    {
        return GL_PIXEL_PACK_BUFFER;
    }

    return 0;
}

// ****************************
//
GLenum  PdrPBOMemTransfer::PBOUsage     ( DataBuffer::Semantic semantic ) const
{
    if( semantic == DataBuffer::Semantic::S_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_DYNAMIC || semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_WRITE )
    {
        return ConstantsMapper::GLConstant( semantic );
    }
    else if ( semantic == DataBuffer::Semantic::S_TEXTURE_STREAMING_READ )
    {
        return ConstantsMapper::GLConstant( semantic );
    }

    return 0;
}

} //bv
