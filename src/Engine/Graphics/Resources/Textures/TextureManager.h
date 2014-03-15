#pragma once

#include <hash_map>
#include <string>

#include "gl/glew.h"

namespace bv
{

namespace model
{
    class ResourceHandle;
}

class Texture;
class Texture2D;
class Texture2DImpl;
class Texture2DSequenceImpl;

class TextureManager
{
    typedef std::hash_map< const bv::Texture *, bv::Texture * > TexMap;

    TexMap  m_txMap;

public:
    
                            TextureManager          ();
    virtual                 ~TextureManager         ();

    Texture2D *             LoadTexture             ( const model::ResourceHandle * resHandle, bool loadFromMemory = true );
    void                    FreeTexture             ( Texture * tx );

    bool                    AddFrame                ( Texture2DSequenceImpl * anim, const model::ResourceHandle * resHandle, bool loadFromMemory = true );

    Texture2DSequenceImpl * CreateEmpty2DSequence   ( const model::ResourceHandle * resHandle );

};

extern TextureManager GTextureManager;

} // bv
