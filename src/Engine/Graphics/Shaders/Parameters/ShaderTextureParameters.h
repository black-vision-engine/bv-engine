#pragma once

#include <vector>


namespace bv {

class Texture2D;
class TextureAnimatedSequence2D;

class ShaderTextureParameters
{
private:

    std::vector< Texture2D * >                  m_textures;

public:

                    ShaderTextureParameters     ();
                    ~ShaderTextureParameters    ();

    void            AddTexture                  ( Texture2D * tx );
    unsigned int    NumTextures                 () const
    {
        return m_textures.size();
    }
    
    const std::vector< Texture2D * > & Textures     () const
    {
        return m_textures;
    }

};

} //bv
