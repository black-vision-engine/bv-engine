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

    int             GetWidth            () const { return m_width; }
    int             GetHeight           () const { return m_height; }
    int             GetSize             () const { return m_size; }
    const char*     GetData             () const { return m_data; }
    char*           GetWritableData     () const { return m_data; }
    
    ResourceHandle( char* data, int size, int w, int h )
        : m_data(data)
        , m_size(size)
        , m_width(w)
        , m_height(h)
    {

    }
};

class IResourceLoader
{
public:
    virtual ResourceHandle *        LoadResource        ( IResource* res )  const    = 0;

    virtual ~IResourceLoader(){}
};

} // model
} // bv