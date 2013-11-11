#pragma once

#include "Engine/Graphics/Resources/DataBuffer.h"
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

                                    Texture         ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE );
    virtual			                ~Texture	    ();

    virtual TextureFormat           GetFormat	    () const;
    virtual TextureType             GetType         () const;
    virtual DataBuffer::Semantic    GetSemantic     () const;

    virtual int                     GetPixelSize    () const;
    static int                      GetPixelSize    ( TextureFormat format );

    virtual size_t                  GetDataSize     () const;

    virtual char *                  GetData         ();
    virtual const char *            GetData         () const;

    virtual bool                    WriteToBuffer   ( const char * memPtr, size_t dataSize );

    virtual bool                    HasSequence     () const;
};

}


