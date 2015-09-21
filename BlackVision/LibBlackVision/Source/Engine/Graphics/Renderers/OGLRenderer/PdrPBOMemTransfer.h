#pragma once

#include "BVGL.h"

#include "Mathematics/glm_inc.h"

#include "Engine/Graphics/Resources/DataBuffer.h"

#include "Engine/Graphics/Renderers/OGLRenderer/PdrConstants.h"

namespace bv {

class PdrPBOMemTransfer
{
protected:

    GLuint					m_numPBOs;

	std::vector< GLuint >	m_pboID;

    GLenum					m_pboTarget;
    GLenum					m_pboUsage;

    GLenum					m_dataSize;
    GLuint					m_index;

public:
    static bool		PBORequired		( DataBuffer::Semantic semantic );

protected:
				PdrPBOMemTransfer   ( DataBuffer::Semantic semantic, SizeType dataSize );
    virtual		~PdrPBOMemTransfer  () = 0;

    GLuint      NumPBOs         ( DataBuffer::Semantic semantic ) const;

    GLenum      PBOTarget       ( DataBuffer::Semantic semantic ) const;
    GLenum      PBOUsage        ( DataBuffer::Semantic semantic ) const;

};

// ******************************
//
class PdrUploadPBO : public PdrPBOMemTransfer
{
public:
			PdrUploadPBO		( DataBuffer::Semantic semantic, SizeType dataSize );

	void 	LockUpload			( const char * source, SizeType dataSize );
	void 	UnlockUpload		();

};

DEFINE_UPTR_TYPE( PdrUploadPBO )

// ******************************
//
class PdrDownloadPBO : public PdrPBOMemTransfer
{
public:
			PdrDownloadPBO		( DataBuffer::Semantic semantic, SizeType dataSize );

	void 	LockDownload		();
	void 	UnlockDownload		( char * dest, SizeType dataSize );

};

DEFINE_UPTR_TYPE( PdrDownloadPBO )

} //bv
