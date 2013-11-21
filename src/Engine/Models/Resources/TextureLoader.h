#pragma once

#include "Engine/Models/Resources/ResourceLoader.h"

#include "Engine/Models/Resources/Resource.h"
#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class TextureLoader : public ResourceLoader
{
    bool                    m_loadFromMemory;
    

public:
    ResourceHandle *        LoadResource        ( IResource* res )  const;

    explicit                TextureLoader( bool loadFormMemory = true );

    virtual ~TextureLoader(){}
};

class TextureExtraData : public ResourceExtraData
{
    int                     m_width;
    int                     m_height;
    int                     m_bitsPerPixel;
    TextureFormat           m_format;
    TextureType             m_type;

public:

    int                     GetWidth            () const;
    int                     GetHeight           () const;
    int                     GetBitsPerPixel     () const;
    TextureFormat           GetFormat           () const;
    TextureType             GetType             () const;

    explicit                TextureExtraData    (int w, int h, int bitsPerPixel, TextureFormat format, TextureType type );

};

} // model
} // bv
