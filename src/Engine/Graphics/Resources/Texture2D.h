#pragma once

#include "Engine/Graphics/Resources/Texture.h"


namespace bv
{

class Texture2D : public Texture
{
private:

    int         m_width;
    int         m_height;

public:

                    Texture2D      ( TextureFormat format, int width, int height, DataBuffer::Semantic semantic );
    virtual         ~Texture2D      () = 0;

    int             GetWidth        () const;
    int             GetHeight       () const;

    unsigned int    RawFrameSize    () const;

protected:

    void            SetWidth        ( unsigned int width );
    void            SetHeight       ( unsigned int height );

};

} //bv
