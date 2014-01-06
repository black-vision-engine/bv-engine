#pragma once

#include <string>

#include "Engine/Models/Plugins/Interfaces/IValue.h"


namespace bv { namespace model {

// *************************************
class NamedValue : public IValue
{
private:

    std::string     m_name;

protected:

    explicit        NamedValue  ( const std::string & name );
    virtual         ~NamedValue ();

public:

    virtual const std::string & GetName () const;

};

// *************************************
template< typename ValueType, ParamType paramType >
class ValueImpl : public NamedValue
{
private:

    ValueType m_value;

public:

            ValueImpl   ( const std::string & name );
    virtual ~ValueImpl  ();

    virtual ParamType       GetParamType    () const override;
    virtual const char *    GetData         () const override;

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
ParamType   ValueImpl< ValueType, paramType >::GetParamType () const
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

} //model
} //bv
