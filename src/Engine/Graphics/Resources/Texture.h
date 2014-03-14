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

    bool                    m_changed;

public:

                            Texture         ( TextureFormat format, TextureType type, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE );
    virtual			        ~Texture	    () = 0;

    virtual size_t          GetDataSize     () const = 0;

    virtual char *          GetData         () = 0;
    virtual const char *    GetData         () const = 0;

    void                    SetChanged      ( bool changed );
    bool                    Changed         () const;

    friend class TextureAccessor;
};


class TextureAccessor
{
public:

    static bool    WriteData( Texture * tx, const char * data, size_t dataSize );

};

} //bv
