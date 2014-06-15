#pragma once

#include <string>

#include "Engine/Interfaces/IValue.h"

namespace bv {

// *************************************
class NamedValue : public IValue, public std::enable_shared_from_this< NamedValue >
{
private:

    std::string     m_name;

protected:

    explicit        NamedValue  ( const std::string & name );
    virtual         ~NamedValue ();

public:

    virtual const std::string & GetName () const override;

};

// *************************************
template< typename ValType, ParamType paramType >
class ValueImpl : public NamedValue
{
public:

    typedef ValType ValueType;

private:

    ValType     m_value;

public:

    explicit                ValueImpl       ( const std::string & name );

public:

    virtual                 ~ValueImpl      ();

    virtual ParamType       GetType         () const override;
    virtual const char *    GetData         () const override;

    virtual VoidPtr         QueryValueTyped () override;

    // *******************************
    //
    const ValType &         GetValue        () const
    {
        return m_value;
    }

    // *******************************
    //
    void                    SetValue        ( const ValType & v )
    {
        m_value = v;
    }
 
    // *******************************
    //
    static ParamType        Type            ()
    {
        return paramType;
    }

    friend class ValuesFactory;
};


// *******************************
//
template< typename ValType, ParamType paramType >
ValueImpl< ValType, paramType >::ValueImpl( const std::string & name )
    : NamedValue( name )
{
}

// *******************************
//
template< typename ValType, ParamType paramType >
ValueImpl< ValType, paramType >::~ValueImpl()
{
}

// *******************************
//
template< typename ValType, ParamType paramType >
ParamType   ValueImpl< ValType, paramType >::GetType      () const
{
    return paramType;
}

// *******************************
//
template< typename ValType, ParamType paramType >
const char *    ValueImpl< ValType, paramType >::GetData  () const
{
    return reinterpret_cast< const char * >( &m_value );
}

// *******************************
//
template< typename ValType, ParamType paramType >
VoidPtr          ValueImpl< ValType, paramType >::QueryValueTyped ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

} //bv
