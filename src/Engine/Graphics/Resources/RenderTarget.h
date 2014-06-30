#pragma once

#include <vector>

#include "Engine/Graphics/Resources/Texture2D.h"

namespace bv
{

//FIXME: add stencil at some point
class RenderTarget
{
public:

    enum class RTSemantic : int 
    {
        S_DRAW_ONLY,
        S_DRAW_READ,
        S_TOTAL
    };

private:

    typedef std::vector< Texture2D * > TexturesVec;

protected:
 
    int                         m_numTargets;
    
    bool                        m_hasMipmaps;
    bool                        m_hasDepthBuffer;

    RenderTarget::RTSemantic    m_semantic;

    TexturesVec                 m_ColorTextures;

public:

                        RenderTarget    ( const std::vector< TextureFormat > & formats, int w, int h, bool hasDepthBuffer, bool hasMipmaps = false, RenderTarget::RTSemantic semantic = RenderTarget::RTSemantic::S_DRAW_ONLY );
    virtual             ~RenderTarget   ();

    RenderTarget::RTSemantic  Semantic        () const;

    int                     NumTargets      () const;
    TextureFormat           Format          () const;

    int                     Width           () const;
    int                     Height          () const;

    Texture2D *             ColorTexture    ( int i ) const;

    bool                    HasMipmaps      () const;
    bool                    HasDepthBuffer  () const;

};

} //bv
