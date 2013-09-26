#pragma once

#include "DataBuffer.h"

namespace bv
{

class Texture
{
protected:

    static int              m_sPixelSize[ TFormat::F_TOTAL ];

    TFormat                 m_format;
    TType                   m_type;
    DataBuffer::Semantic    m_semantic;

    char *                  m_data;
    int                     m_dataSize;

public:

                            Texture         ( Texture::TFormat format, Texture::TType type, DataBuffer::Semantic semantic =  DataBuffer::Semantic::S_TEXTURE );

    virtual			        ~Texture	    ();

    TFormat                 GetFormat	    () const;
    TType                   GetType         () const;
    DataBuffer::Semantic    GetSemantic     () const;

    int                     GetPixelSize    () const;
    static int              GetPixelSize    ( TFormat format );

    size_t                  GetDataSize     () const;

    char *                  GetData         ();
    const char *            GetData         () const;

    bool                    WriteToBuffer   ( const char * memPtr, size_t dataSize );

};

}


