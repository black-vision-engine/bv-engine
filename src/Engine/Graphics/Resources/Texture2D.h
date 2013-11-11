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

                 Texture2D       ( TextureFormat format, int width, int height, DataBuffer::Semantic semantic = DataBuffer::Semantic::S_TEXTURE );
    virtual     ~Texture2D      ();

    int         GetWidth        () const;
    int         GetHeight       () const;

};

}
