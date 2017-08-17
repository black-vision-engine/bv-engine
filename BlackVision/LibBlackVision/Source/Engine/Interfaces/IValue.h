#pragma once

#include <cassert>
#include <string>
#include <memory>

#include "Engine/Types/Enums.h"
#include "CoreDEF.h"


namespace bv {

class IValue;
typedef std::shared_ptr< const IValue > IValueConstPtr;
DEFINE_PTR_TYPE(IValue)

class IValue
{
public:

    virtual ParamType           GetType         ()  const = 0;
    virtual const char *        GetData         ()  const = 0;
    virtual SizeType            GetSize         ()  const = 0;
    virtual const std::string & GetName         ()  const = 0;  
    virtual void                SetName         ( const std::string & ) = 0;

    virtual VoidPtr             QueryValueTyped () = 0;
	virtual void *				QueryThis		() = 0;

    virtual ~IValue(){}

};

typedef std::vector< IValuePtr > IValuePtrVec;

// *********************************
//
template< typename ValueTypePtr >
inline ValueTypePtr QueryTypedValue( IValuePtr val )
{
#ifdef _DEBUG
    if( val->GetType() != ValueTypePtr::element_type::Type() )
    {
        return nullptr;
    }
#endif
    return std::static_pointer_cast< ValueTypePtr::element_type >( val->QueryValueTyped() );
}

// *********************************
//
template< typename ValueTypeConstPtr >
inline ValueTypeConstPtr QueryTypedValue( IValueConstPtr val )
{
#ifdef _DEBUG
    if( val->GetType() != ValueTypeConstPtr::element_type::Type() )
    {
        return nullptr;
    }
#endif
    return std::static_pointer_cast< ValueTypeConstPtr::element_type >( std::const_pointer_cast< IValue >( val )->QueryValueTyped() );
}


// *********************************
//
template< typename ValueType >
inline ValueType * QueryTypedValue( IValue * val )
{
#ifdef _DEBUG
    if( val->GetType() != ValueType::Type() )
    {
        return nullptr;
    }
#endif
    return static_cast< ValueType * >( val->QueryThis() );
}

// *********************************
//
template< typename ValueType >
inline const ValueType * QueryTypedValue( const IValue * val )
{
#ifdef _DEBUG
    if( val->GetType() != ValueType::Type() )
    {
        return nullptr;
    }
#endif
    return static_cast< const ValueType * >( const_cast< IValue * >( val )->QueryThis() );
}

} // bv
