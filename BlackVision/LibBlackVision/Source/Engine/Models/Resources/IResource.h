#pragma once

#include <string>
#include "System/BasicTypes.h"
#include "Core/MemoryChunk.h"

namespace bv { namespace model
{

class IResourceNEW;
DEFINE_CONST_PTR_TYPE( IResourceNEW )

class IResourceNEW
{
protected:
	virtual VoidConstPtr					QueryThis	() const = 0;

public:

	virtual const std::string &				GetUID		() const = 0;

    virtual ~IResourceNEW(){}

	template< typename ResourceTypeConstPtr >
	friend ResourceTypeConstPtr  QueryTypedRes( ResourceTypeConstPtr res );
};

template< typename ResourceTypeConstPtr >
ResourceTypeConstPtr  QueryTypedRes( IResourceNEWConstPtr res )
{
	if( res->GetUID() != ResourceTypeConstPtr::element_type::UID() )
    {
        return nullptr;
    }

    return std::static_pointer_cast< ResourceTypeConstPtr::element_type >( res->QueryThis() );
}



//class IResource
//{
//public:
//
//    virtual const std::string &     GetName     () const = 0;
//    virtual const std::string &     GetFilePath () const = 0;
//
//    virtual ~IResource(){}
//
//};
//
//enum class ResourceExtraKind : int
//{
//    RE_TEXTURE = 0,
//    RE_FONT,
//    RE_TOTAL
//};
//
//class IResourceExtraData
//{
//public:
//
//    virtual ResourceExtraKind       GetResourceExtraKind        () const = 0;
//
//    virtual ~IResourceExtraData() {}
//};
//
//
//class ResourceHandle
//{
//public: // Only for non intrusive serialization. Should be private
//
//    SizeType				m_size;
//    MemoryChunkConstPtr     m_data;
//
//    IResourceExtraData *    m_extra;
//
//public:
//
//    SizeType					GetSize             () const { return m_size; }
//    MemoryChunkConstPtr         GetData             () const { return m_data; }
//    MemoryChunkConstPtr         GetWritableData     () const { return m_data; }
//    const IResourceExtraData *  GetExtra            () const { return m_extra; }
//    void                        SetData             ( MemoryChunkConstPtr pData ) { m_data = pData; }
//
//    ResourceHandle()
//        : m_data( nullptr )
//        , m_size( 0 )
//        , m_extra( nullptr )
//    {}
//
//    ResourceHandle( MemoryChunkConstPtr data, SizeType size, IResourceExtraData * extra = nullptr )
//        : m_data( data )
//        , m_size( size )
//        , m_extra( extra )
//    {
//    }
//
//    ~ResourceHandle()
//    {
//        delete m_extra;
//    }
//};
//
//DEFINE_PTR_TYPE(ResourceHandle)
//DEFINE_CONST_PTR_TYPE(ResourceHandle)

} // model
} // bv
