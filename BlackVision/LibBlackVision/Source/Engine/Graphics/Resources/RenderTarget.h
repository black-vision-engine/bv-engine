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

    typedef std::vector< Texture2DPtr > TexturesVec;

protected:
 
    UInt32                      m_numTargets;
    
    bool                        m_hasMipmaps;
    bool                        m_hasDepthBuffer;

    RenderTarget::RTSemantic    m_semantic;

    TexturesVec                 m_ColorTextures;

public:

                RenderTarget    ( const std::vector< TextureFormat > & formats, UInt32 w, UInt32 h, bool hasDepthBuffer, bool hasMipmaps = false, RenderTarget::RTSemantic semantic = RenderTarget::RTSemantic::S_DRAW_ONLY );
    virtual     ~RenderTarget   ();

    RenderTarget::RTSemantic    Semantic        () const;

    UInt32					    NumTargets      () const;
    TextureFormat               Format          () const;

    UInt32					    Width           () const;
    UInt32					    Height          () const;

    Texture2DPtr                ColorTexture    ( UInt32 i ) const;

    bool                        HasMipmaps      () const;
    bool                        HasDepthBuffer  () const;

};

} //bv
