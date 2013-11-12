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
    if( !params.m_containsAnimations )
    {
        params.m_containsTextures = true;

        params.AddTexture( tx );

        return true;
    }

    return false;
}

// *********************************
//  
bool                                                ShaderTextureParametersAccessor::Add                ( ShaderTextureParameters & params, TextureAnimatedSequence2D * anim )
{
    if( !params.m_containsTextures )
    {
        params.m_containsAnimations= true;

        params.AddAnimation( anim );

        return true;
    }

    return false;
}

// *********************************
//  
const std::vector< Texture2D * > *                  ShaderTextureParametersAccessor::GetTextures        ( const ShaderTextureParameters & params )
{
    return params.m_containsTextures ? &params.m_textures : nullptr;
}

// *********************************
//  
const std::vector< TextureAnimatedSequence2D * > *  ShaderTextureParametersAccessor::GetAnimations      ( const ShaderTextureParameters & params )
{
    return params.m_containsAnimations ? &params.m_animations : nullptr;
}

// *********************************
//  
bool                                                ShaderTextureParametersAccessor::ContainsTextures   ( const ShaderTextureParameters & params )
{
    return params.m_containsTextures;
}

// *********************************
//  
bool                                                ShaderTextureParametersAccessor::ContainsAnimations ( const ShaderTextureParameters & params )
{
    return params.m_containsAnimations;
}

// *********************************
//  
unsigned int                                        ShaderTextureParametersAccessor::NumEntries         ( const ShaderTextureParameters & params )
{
    if( params.m_containsAnimations )
    {
        return params.m_animations.size();
    }
    else if ( params.m_containsTextures )
    {
        return params.m_textures.size();
    }

    return 0;
}

} //bv
