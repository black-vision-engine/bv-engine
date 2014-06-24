#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"

namespace bv
{

//FIXME: add stencil at some point
class RenderTarget
{
private:

    typedef std::vector< Texture2D * > TexturesVec;

protected:
 
    int             m_numTargets;
    
    bool            m_hasMipmaps;
    bool            m_hasDepthBuffer;

    TexturesVec     m_ColorTextures;

public:

                        RenderTarget    ( const std::vector< TextureFormat > & formats, int w, int h, bool hasDepthBuffer, bool hasMipmaps = false );
    virtual             ~RenderTarget   ();

    int                 NumTargets      () const;
    TextureFormat       Format          () const;

    int                 Width           () const;
    int                 Height          () const;

    Texture2D *         ColorTexture    ( int i ) const;

    bool                HasMipmaps      () const;
    bool                HasDepthBuffer  () const;

};

} //bv
