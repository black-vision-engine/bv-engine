#pragma once

#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"

#include <cassert>

namespace bv
{

class Texture
{
protected:
    static int							m_sPixelSize[ TextureFormat::F_TOTAL ];

    DataBuffer::Semantic				m_semantic;
	
    TextureFormat						m_format;
    TextureType							m_type;

	UInt32								m_levels;

    std::vector< MemoryChunkConstPtr >	m_data;

	UInt32								m_updateID;

public:

                                Texture         ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic, UInt32 levels, UInt32 dataNum );
    virtual			            ~Texture	    () = 0;

	UInt32						GetNumLevels    () const;

	TextureFormat				GetFormat	    () const;
    TextureType					GetType         () const;

    DataBuffer::Semantic        GetSemantic     () const;

	SizeType					GetPixelSize    () const;
    static SizeType				GetPixelSize    ( TextureFormat format );

	UInt32						GetUpdateID		() const;
	void						ForceUpdate		();
	
protected:

	MemoryChunkConstPtr		GetDataChunk    ( UInt32 idx ) const;
	void					SetDataChunk    ( MemoryChunkConstPtr data, UInt32 idx );

    void                    SetFormat       ( TextureFormat format );
    void                    SetType         ( TextureType type );

};

DEFINE_PTR_TYPE(Texture)
DEFINE_CONST_PTR_TYPE(Texture)

} //bv
