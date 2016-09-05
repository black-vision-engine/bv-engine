#pragma once

#include <string>

#include "Engine/Types/Enums.h"
#include "Engine/Types/Values/TypedValues.h"


namespace bv {

class RenderableEntity;
class Camera;

class GenericShaderParam
{
private:

    mutable int         m_id;
    
    mutable bool        m_updatable;
    
    ParamType			m_paramType;
    std::string         m_paramName;

protected:

                                GenericShaderParam  ( ParamType type, const std::string & name );
public:

    virtual                     ~GenericShaderParam ();

    void                        SetIntID            ( int id ) const;
    void                        SetUpdatable        ( bool updatable ) const;

    inline  int                 IntID               () const;
    inline ParamType            Type                () const;
    inline const std::string &  Name                () const;
    inline bool                 IsUpdatable         () const;

    virtual void                Update              ( RenderableEntity * renderable, Camera * camera );

    template< typename ValType >
    inline const ValType &      GenericGetValue     () const;
    
protected:

    virtual const void *        GetValuePtr         () const = 0;

};


// *****************************************
template< ParamType paramType >
struct ShaderValueTypeTraits
{
    typedef void ValueType;
};

// *****************************************
template< typename ValueType >
struct ShaderParamTypeTraits
{
    static const ParamType paramType = ParamType::PT_TOTAL;
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
