#pragma once

#include <hash_map>
#include <string>

#include "gl/glew.h"

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Models/Interfaces/IAnimationDescriptor.h"

#include "Engine/Graphics/Resources/Texture2D.h"

#include "Engine/Graphics/Resources/Texture2DImpl.h"
#include "Engine/Graphics/Resources/Texture2DSequenceImpl.h"


namespace bv
{

namespace model
{
    class ResourceHandle;
}

//FIXME: right now used only to cache static textures
class Texture2DCache
{
    typedef std::hash_map< ITextureDescriptor::uid_t, Texture2DPtr >    Texture2DMap;

    Texture2DMap  m_tex2DCache;

public:
  
                                Texture2DCache          ();
    virtual                     ~Texture2DCache         ();

    Texture2DPtr                GetTexture              ( const ITextureDescriptor * txParams );
    Texture2DPtr                GetAnimation            ( const IAnimationDescriptor * animParams );

    void                        ClearCache              ();

    static Texture2DImplPtr         CreateEmptyTexture  ( TextureFormat format, unsigned int width, unsigned int height, DataBuffer::Semantic semantic );
    static Texture2DSequenceImplPtr CreateEmptySequence ( TextureFormat format, unsigned int width, unsigned int height  );

};

extern Texture2DCache GTexture2DCache;

} // bv
