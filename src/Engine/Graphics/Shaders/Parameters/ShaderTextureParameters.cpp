#include "ShaderTextureParameters.h"

#include <cassert>


namespace bv {

// ******************************************* ShaderTextureParameters *******************************************

// *********************************
//  
ShaderTextureParameters::ShaderTextureParameters    ()
    : m_containsAnimations( false )
    , m_containsTextures( false )
{
}

// *********************************
//  
ShaderTextureParameters::~ShaderTextureParameters   ()
{
}

// *********************************
//  
void    ShaderTextureParameters::AddTexture         ( Texture2D * tx )
{
    m_textures.push_back( tx );
}

// *********************************
//  
void    ShaderTextureParameters::AddAnimation       ( TextureAnimatedSequence2D * anim )
{
    m_animations.push_back( anim );
}

// ******************************************* ShaderTextureParametersAccessor *******************************************

// *********************************
//  
bool                                                ShaderTextureParametersAccessor::Add                ( ShaderTextureParameters & params, Texture2D * tx )
{
    params.AddTexture( tx );

    return true;
}

// *********************************
//  
const std::vector< Texture2D * > *                  ShaderTextureParametersAccessor::GetTextures        ( const ShaderTextureParameters & params )
{
    return &params.m_textures;
}

// *********************************
//  
unsigned int                                        ShaderTextureParametersAccessor::NumEntries         ( const ShaderTextureParameters & params )
{
    return params.m_textures.size();
}

} //bv
