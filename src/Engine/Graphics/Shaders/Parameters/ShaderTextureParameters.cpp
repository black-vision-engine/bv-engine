#include "ShaderTextureParameters.h"

#include <cassert>


namespace bv {

// *********************************
//  
ShaderTextureParameters::ShaderTextureParameters    ()
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

} //bv
