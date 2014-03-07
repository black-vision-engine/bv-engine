#pragma once

#include <string>

#include "Engine/Types/Enums.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv {

template< ParamType paramType >
struct ShaderValueTypeTraits
{
    typedef void ValueType;
};

template< typename ValueType >
struct ShaderParamTypeTraits
{
    static const ParamType paramType = ParamType::PT_TOTAL;
};


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


#define SPECIALIZE_SHADER_TRAIT( parameterType, valueType ) \
template<> \
struct ShaderValueTypeTraits< parameterType > \
{ \
    typedef valueType ValueType; \
}; \
template<> \
struct ShaderParamTypeTraits< valueType > \
{ \
    static const ParamType paramType = parameterType; \
}


} //bv

#include "GenericShaderParam.inl"
