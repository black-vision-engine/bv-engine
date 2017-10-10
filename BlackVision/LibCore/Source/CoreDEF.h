#pragma once

// @see http://stackoverflow.com/questions/1505582/determining-32-vs-64-bit-in-c
// Check windows
#if _WIN32 || _WIN64
   #if _WIN64
     #define ENV64BIT
  #else
    #define ENV32BIT
  #endif
#endif


#include <memory>
#include <vector>
#include <string>
#include <cassert>

#define DEFINE_PTR_TYPE(Type) typedef std::shared_ptr< Type > Type##Ptr;
#define DEFINE_CONST_PTR_TYPE(Type) typedef std::shared_ptr< const Type > Type##ConstPtr;

#define DEFINE_WEAK_PTR_TYPE(Type) typedef std::weak_ptr< Type > Type##WeakPtr;

#define DEFINE_UPTR_TYPE(Type) typedef std::unique_ptr< Type > Type##UPtr;

#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)

#define TEST_ACCESSOR( className ) _TEST_ACCESSOR_ ## className
#define FRIEND_TEST_ACCESSOR( className ) friend class TEST_ACCESSOR( className );

typedef std::shared_ptr< void > VoidPtr;
typedef std::shared_ptr< const void > VoidConstPtr;

enum MouseAction { RIGHT_DOWN, RIGHT_UP, LEFT_DOWN, LEFT_UP, MOVE };

namespace bv {

typedef float				Float32;
typedef double				Float64;

typedef signed char			Int8;
typedef signed short		Int16;
typedef signed long long	Int64;
typedef signed int			Int32;

typedef unsigned char		UInt8;
typedef unsigned short		UInt16;
typedef unsigned long long	UInt64;
typedef unsigned int		UInt32;
typedef size_t				SizeType;
typedef std::ptrdiff_t      PtrOffset;

typedef Float32				TimeType;

typedef std::vector< std::string > StringVector;


// ************************
//
template< class DestType, class SrcType >
DestType Cast( SrcType * ptr )
{
    assert( dynamic_cast< DestType >( ptr ) );
    return static_cast< DestType >( ptr );
}

// ************************
//
template< class Type >
Type * RemoveConst( const Type * ptr )
{
    return const_cast< Type * >( ptr );
}

} //bv
