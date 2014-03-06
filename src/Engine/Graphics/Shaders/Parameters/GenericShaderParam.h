#pragma once

#include <string>

#include "Engine/Types/Enums.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class GenericShaderParam
{
private:

    ParamType           m_paramType;
    std::string         m_paramName;

protected:

                                GenericShaderParam  ( ParamType type, const std::string & name );
public:

    virtual                     ~GenericShaderParam ();

    inline ParamType            Type                () const;
    inline const std::string &  Name                () const;

    virtual void                Update              ();

    template< typename ValType >
    const ValType &             GenericGetValue     () const;

protected:

    virtual const void *        GetValuePtr         () const = 0;

};

} //bv

#include "GenericShaderParam.inl"
