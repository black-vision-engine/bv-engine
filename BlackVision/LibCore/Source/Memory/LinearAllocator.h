#pragma once

#include "MemoryChunk.h"

#include <vector>


namespace bv
{


class LinearAllocator
{
private:

    std::vector< MemoryChunk >  m_memChunks;
    PtrOffset                   m_offset;

public:
    explicit    LinearAllocator ( SizeType initSize );
                ~LinearAllocator();


    template< typename Type >
    Type *      Allocate    ();

    template< typename Type >
    void        Free        ( Type* ptr );
    void        FreeAll     ();

private:
    template< typename Type >
    Type*       GetNextAlignedPtr   ();

    void        AllocNewChunk       ();
};


// ***********************
//
template< typename Type >
inline Type *       LinearAllocator::Allocate()
{
    Type * newObject = GetNextAlignedPtr< Type >();
    if( !newObject )
    {
        AllocNewChunk();
        return GetNextAlignedPtr< Type >();
    }

    return newObject;
}

// ***********************
// Calls destructor for object.
template< typename Type >
inline void         LinearAllocator::Free       ( Type * ptr )
{
    ptr->~Type();
}

// ***********************
//
template< typename Type >
inline Type *       LinearAllocator::GetNextAlignedPtr  ()
{
    auto size = sizeof( Type );
    auto alignement = std::alignment_of< Type >::value;
    auto space = m_memChunks.back().Size() - m_offset;
    void* ptr = m_memChunks.back().GetWritable() + m_offset;

    Type* newPtr = static_cast< Type* >( std::align( alignement, size, ptr, space ) );

    m_offset = (const char *)newPtr + size - m_memChunks.back().Get();

    return newPtr;
}

}	// bv