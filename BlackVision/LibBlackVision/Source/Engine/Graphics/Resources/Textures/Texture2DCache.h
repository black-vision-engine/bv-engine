#pragma once

#include <hash_set>
#include <hash_map>
#include <string>

#include "Engine/Models/Interfaces/ITextureDescriptor.h"
#include "Engine/Models/Interfaces/IAnimationDescriptor.h"

#include "Engine/Graphics/Resources/Texture2D.h"

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
    typedef std::hash_set< const Texture2D * >                          Texture2DSet;

    Texture2DMap        m_tex2DCache;
    Texture2DSet        m_tex2DSet;

public:
  
                                    Texture2DCache      ();
    virtual                         ~Texture2DCache     ();

    Texture2DPtr                    GetTexture          ( const ITextureDescriptor * txParams );
	//@SEQUENCE
    Texture2DPtr                    GetSequence         ( const IAnimationDescriptor * animParams );

    bool                            IsRegistered        ( const ITextureDescriptor * txParams ) const;
    bool                            IsStored            ( Texture2DPtr tex ) const;

    void                            ClearCache          ();

    static Texture2DPtr				CreateEmptyTexture  ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic );
    static Texture2DPtr				CreateTexture       ( TextureFormat format, UInt32 width, UInt32 height, DataBuffer::Semantic semantic, MemoryChunkConstPtr data );

	//@SEQUENCE
    //static Texture2DSequenceImplPtr CreateEmptySequence ( TextureFormat format, UInt32 width, UInt32 height  );

};

extern Texture2DCache GTexture2DCache;

} // bv
