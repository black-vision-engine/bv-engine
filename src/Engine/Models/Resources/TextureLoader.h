#pragma once

#include "ResourceLoader.h"
#include "Engine/Models/Resources/Resource.h"

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

public:

    int                     GetWidth            () const;
    int                     GetHeight           () const;

    explicit                TextureExtraData    ( int w, int h );

};

} // model
} // bv