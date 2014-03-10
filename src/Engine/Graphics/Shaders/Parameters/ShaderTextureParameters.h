#pragma once

#include <vector>


namespace bv {

class Texture2D;
class TextureAnimatedSequence2D;


class ShaderTextureParameters
{
private:

    std::vector< Texture2D * >                  m_textures;
    std::vector< TextureAnimatedSequence2D * >  m_animations;

    bool                                        m_containsTextures;
    bool                                        m_containsAnimations;

public:

            ShaderTextureParameters     ();
            ~ShaderTextureParameters    ();

private:

    void    AddTexture                  ( Texture2D * tx );            
    void    AddAnimation                ( TextureAnimatedSequence2D * anim );
    
    //FIXME: add other texture types as well and think of better implementation of ShaderTextureParameters for 1D, 3D and CUBIC textures (by means of templates or something - to be thought over)
    friend class ShaderTextureParametersAccessor;

};

class ShaderTextureParametersAccessor
{
public:

    static bool                                                 Add                 ( ShaderTextureParameters & params, Texture2D * tx );
    static bool                                                 Add                 ( ShaderTextureParameters & params, TextureAnimatedSequence2D * anim );
                                                                                                      
    static const std::vector< Texture2D * > *                   GetTextures         ( const ShaderTextureParameters & params );
    static const std::vector< TextureAnimatedSequence2D * > *   GetAnimations       ( const ShaderTextureParameters & params );
                                                                                                      
    static bool                                                 ContainsTextures    ( const ShaderTextureParameters & params );
    static bool                                                 ContainsAnimations  ( const ShaderTextureParameters & params );

    static unsigned int                                         NumEntries          ( const ShaderTextureParameters & params );
};

} //bv
