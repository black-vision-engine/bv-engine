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
public:

    SizeType				m_width;
    SizeType				m_height;
    SizeType				m_bitsPerPixel;

    TextureFormat           m_format;
    TextureType             m_type;

public:

    SizeType                GetWidth            () const;
    SizeType                GetHeight           () const;
    SizeType                GetBitsPerPixel     () const;

    TextureFormat           GetFormat           () const;
    TextureType             GetType             () const;

                            TextureExtraData    ();
    explicit                TextureExtraData    ( SizeType w, SizeType h, SizeType bitsPerPixel, TextureFormat format, TextureType type );

};

} // model
} // bv
