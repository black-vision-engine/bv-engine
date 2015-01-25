#pragma once

#include "Engine/Graphics/Resources/TextureBase.h"
#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Engine/Types/Enums.h"
#include "Core/MemoryChunk.h"

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

    virtual size_t              GetDataSize     () const = 0;

    virtual MemoryChunkConstPtr GetData         () const = 0;

    DataBuffer::Semantic        GetSemantic     () const;

    void                        SetChanged      ( bool changed ) const;
    bool                        Changed         () const;

};

} //bv
