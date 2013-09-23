#pragma once

#include "ResourceLoader.h"

namespace bv { namespace model {

class TextureLoader : public ResourceLoader
{
    bool                    m_loadFromMemory;

public:
    ResourceHandle *        LoadResource        ( IResource* res )  const;

    explicit                TextureLoader( bool loadFormMemory = true );

    virtual ~TextureLoader(){}
};

} // model
} // bv