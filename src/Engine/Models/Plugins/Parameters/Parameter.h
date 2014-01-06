#pragma once

#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Interfaces/IValue.h"
#include "Engine/Interfaces/ITimeEvaluator.h"

#include "Engine/Types/Enums.h"


namespace bv { namespace model {

// TODO: Move to another file
class BaseValue : public IValue
{
private:

    std::string             m_name;

protected:

    explicit BaseValue( const std::string & name )
        : m_name( name )
    {
    }

    virtual ~BaseValue(){}

public:

    virtual const std::string &     GetName ()  const { return m_name; }

};



class ValueFloat : public BaseValue
{
private:

    float                   m_value;

public:
    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_FLOAT; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>( &m_value ); }

    float                       GetValue        ()              const   { return m_value; }
    void                        SetValue        ( const float & v )     { m_value = v; }

    explicit                    ValueFloat      ( const std::string & name );

    virtual ~ValueFloat(){}

};


class ValueVec4 : public BaseValue
{
private:

    glm::vec4                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_FLOAT4; }
    virtual const char *        GetData         ()              const   { return reinterpret_cast<const char*>( &m_value ); }

    const glm::vec4 &           GetValue        ()              const   { return m_value; }
    void                        SetValue        ( const glm::vec4 & v ) { m_value = v; }

    explicit                    ValueVec4       ( const std::string & name );

    virtual ~ValueVec4(){}

};

class ValueMat2 : public BaseValue
{
private:

    glm::mat2                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_MAT2; }
    virtual const char*         GetData         ()              const   { return reinterpret_cast<const char*>( &m_value ); }

    const glm::mat2&            GetValue        ()              const   { return m_value; }
    void                        SetValue        ( const glm::mat2 & v ) { m_value = v; }

    explicit                    ValueMat2       ( const std::string & name );

    virtual ~ValueMat2(){}

};

class ValueMat4 : public BaseValue
{
private:

    glm::mat4                   m_value;

public:

    virtual ParamType           GetParamType    ()              const   { return ParamType::PT_MAT4; }
    virtual const char *        GetData         ()              const   { return reinterpret_cast<const char*>( &m_value ); }

    const glm::mat4 &           GetValue        ()              const   { return m_value; }
    void                        SetValue        ( const glm::mat4 & v ) { m_value = v; }

    explicit                    ValueMat4       ( const std::string & name );

    virtual ~ValueMat4(){}

};

} // model
} // bv
