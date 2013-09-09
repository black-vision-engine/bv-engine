#pragma once

#include "Texture.h"
#include <vector>

namespace bv
{

class Texture2D;

class RenderTarget
{
private:

    typedef std::vector< Texture2D * > TexturesVec;

protected:
 
    int         m_numTargets;
    bool        m_hasMipmaps;

    TexturesVec m_ColorTextures;

public:

                        RenderTarget    ( int numTargets, Texture::TFormat fomat, int w, int h, bool hasMipmaps );
    virtual             ~RenderTarget   ();

    int                 NumTargets      () const;
    Texture::TFormat    Format          () const;

    int                 Width           () const;
    int                 Height          () const;

    Texture2D *         ColorTexture    ( int i );

    bool                HasMipmaps      () const;

};

}
