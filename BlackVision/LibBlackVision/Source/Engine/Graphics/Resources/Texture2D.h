#pragma once

#include "Engine/Graphics/Resources/Texture.h"


namespace bv
{

class Texture2D : public Texture
{
private:

    unsigned int    m_width;
    unsigned int    m_height;

public:

                        Texture2D       ( TextureFormat format, unsigned int width, unsigned int height, DataBuffer::Semantic semantic );
    virtual             ~Texture2D      () = 0;

    unsigned int        GetWidth        () const;
    unsigned int        GetHeight       () const;

    unsigned int        RawFrameSize    () const;
    static unsigned int RawFrameSize    ( TextureFormat format, unsigned int width, unsigned int height );

protected:

    void                SetWidth        ( unsigned int width );
    void                SetHeight       ( unsigned int height );

};

DEFINE_PTR_TYPE(Texture2D)
DEFINE_CONST_PTR_TYPE(Texture2D)

} //bv