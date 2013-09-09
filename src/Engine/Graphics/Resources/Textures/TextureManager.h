#pragma once

#include <hash_map>
#include <string>

#include "gl/glew.h"

namespace bv
{

class Texture;

class TextureManager
{
    typedef std::hash_map< const bv::Texture *, bv::Texture * > TexMap;

    TexMap  m_txMap;

public:
    
                TextureManager  ();
    virtual     ~TextureManager ();

    Texture *   LoadTexture     ( const std::string & filename, bool loadFromMemory = true );
    void        FreeTexture     ( Texture * tx );

};

extern TextureManager GTextureManager;

} // bv
