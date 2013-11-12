#pragma once

#include "Engine/Graphics/Resources/TextureBase.h"
#include "Engine/Graphics/Resources/DataBuffer.h"
#include "Engine/Types/Enums.h"


namespace bv
{

class Texture : public TextureBase
{
protected:

    DataBuffer::Semantic    m_semantic;

    char *                  m_data;
    int                     m_dataSize;

public:

                            Texture         ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE );
    virtual			        ~Texture	    ();

    size_t                  GetDataSize     () const;

    char *                  GetData         ();
    const char *            GetData         () const;

    friend class TextureAccessor;
};


class TextureAccessor
{
public:

    static bool    WriteData( Texture * tx, const char * data, size_t dataSize );

};

} //bv
