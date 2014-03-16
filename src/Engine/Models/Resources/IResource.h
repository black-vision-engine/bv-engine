#pragma once

#include <string>


namespace bv { namespace model
{

class IResource
{
public:

    virtual const std::string &     GetName     () const = 0;
    virtual const std::string &     GetFilePath () const = 0;

    virtual ~IResource(){}

};

enum class ResourceExtraKind : int
{
    RE_TEXTURE = 0,
    RE_FONT,
    RE_TOTAL
};

class IResourceExtraData
{
public:

    virtual ResourceExtraKind       GetResourceExtraKind        () const = 0;

    virtual ~IResourceExtraData() {}
};


class ResourceHandle
{
private:

    int                     m_size;
    char *                  m_data;

    IResourceExtraData *    m_extra;

public:

    int                         GetSize             () const { return m_size; }
    const char *                GetData             () const { return m_data; }
    char *                      GetWritableData     () const { return m_data; }
    const IResourceExtraData *  GetExtra            () const { return m_extra; }
    
    ResourceHandle( char * data, int size, IResourceExtraData * extra = nullptr )
        : m_data( data )
        , m_size( size )
        , m_extra( extra )
    {
    }
};

} // model
} // bv