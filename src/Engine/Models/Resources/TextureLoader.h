#pragma once

#include "Engine/Models/Resources/ResourceLoader.h"

#include "Engine/Models/Resources/Resource.h"
#include "Engine/Types/Enums.h"


namespace bv { namespace model {

class TextureLoader : public ResourceLoader
{
private:

    bool                    m_loadFromMemory;

public:

    ResourceHandle *        LoadResource        ( IResource * res )  const;

    explicit                TextureLoader( bool loadFormMemory = true );

    virtual ~TextureLoader(){}
};

class TextureExtraData : public ResourceExtraData
{
    unsigned int            m_width;
    unsigned int            m_height;
    unsigned int            m_bitsPerPixel;
    TextureFormat           m_format;
    TextureType             m_type;

public:

    unsigned int            GetWidth            () const;
    unsigned int            GetHeight           () const;
    unsigned int            GetBitsPerPixel     () const;
    TextureFormat           GetFormat           () const;
    TextureType             GetType             () const;

    explicit                TextureExtraData    ( unsigned int w, unsigned int h, unsigned int bitsPerPixel, TextureFormat format, TextureType type );

};

} // model
} // bv
