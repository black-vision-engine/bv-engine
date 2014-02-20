#pragma once

#include <string>

#include "Engine/Interfaces/IValue.h"


namespace bv {

// *************************************
class NamedValue : public IValue
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
template< typename ValueType, ParamType paramType >
class ValueImpl : public NamedValue
{
private:

    ValueType m_value;

private:

    explicit                ValueImpl       ( const std::string & name );

public:

    virtual                 ~ValueImpl      ();

    virtual ParamType       GetType         () const override;
    virtual const char *    GetData         () const override;

    virtual void *          QueryValueTyped () override;

    // *******************************
    //
    const ValueType &       GetValue        () const
    {
        return m_value;
    }

    // *******************************
    //
    void                    SetValue        ( const ValueType & v )
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
template< typename ValueType, ParamType paramType >
ValueImpl< ValueType, paramType >::ValueImpl( const std::string & name )
    : NamedValue( name )
{
}

// *******************************
//
template< typename ValueType, ParamType paramType >
ValueImpl< ValueType, paramType >::~ValueImpl()
{
}

// *******************************
//
template< typename ValueType, ParamType paramType >
ParamType   ValueImpl< ValueType, paramType >::GetType      () const
{
    return paramType;
}

// *******************************
//
template< typename ValueType, ParamType paramType >
const char *    ValueImpl< ValueType, paramType >::GetData  () const
{
    return reinterpret_cast< const char * >( &m_value );
}

// *******************************
//
template< typename ValueType, ParamType paramType >
void *          ValueImpl< ValueType, paramType >::QueryValueTyped ()
{
    return static_cast< void * >( this );
}

} //bv
