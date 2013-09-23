#pragma once

#include "IResource.h"

namespace bv { namespace model {

class ResourceHandle
{
private:

    int         m_size;
    int         m_width;
    int         m_height;
    char*       m_data;

public:

    int             GetWidth            () const;
    int             GetHeight           () const;
    int             GetSize             () const;
    const char*     GetData             () const;
    char*           GetWritableData     () const;
    
    ResourceHandle( char* data, int size, int w, int h );
};

class IResourceLoader
{
public:
    virtual ResourceHandle *        LoadResource        ( IResource* res )  const    = 0;

    virtual ~IResourceLoader(){}
};

} // model
} // bv