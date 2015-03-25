#pragma once

#include "Engine/Graphics/Resources/TextureBase.h"
#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Engine/Types/Enums.h"
#include "Memory/MemoryChunk.h"

namespace bv
{

class Texture : public TextureBase
{
protected:

    DataBuffer::Semantic    m_semantic;

    mutable bool             m_changed;

public:

                                Texture         ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE_DYNAMIC );
    virtual			            ~Texture	    () = 0;

	virtual UInt32				GetNumLevels    () const = 0;

    virtual SizeType            GetDataSize     ( UInt32 level = 0 ) const = 0;

	virtual MemoryChunkConstPtr GetData         ( UInt32 level = 0 ) const = 0;

    DataBuffer::Semantic        GetSemantic     () const;

    void                        SetChanged      ( bool changed ) const;
    bool                        Changed         () const;

};

} //bv
