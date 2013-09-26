#pragma once

#include "DataBuffer.h"

#include "Engine/Types/Enums.h"

namespace bv
{

class Texture
{
protected:

    static int              m_sPixelSize[ TextureFormat::F_TOTAL ];

    TextureFormat           m_format;
    TextureType             m_type;
    DataBuffer::Semantic    m_semantic;

    char *                  m_data;
    int                     m_dataSize;

public:

                            Texture         ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic =  DataBuffer::Semantic::S_TEXTURE );

    virtual			        ~Texture	    ();

    TextureFormat           GetFormat	    () const;
    TextureType             GetType         () const;
    DataBuffer::Semantic    GetSemantic     () const;

    int                     GetPixelSize    () const;
    static int              GetPixelSize    ( TextureFormat format );

    size_t                  GetDataSize     () const;

    char *                  GetData         ();
    const char *            GetData         () const;

    bool                    WriteToBuffer   ( const char * memPtr, size_t dataSize );

};

}


