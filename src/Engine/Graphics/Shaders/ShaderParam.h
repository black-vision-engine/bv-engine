#pragma once

#include <cassert>
#include <vector>

#include "Engine/Types/Enums.h"
#include "Engine/Graphics/Shaders/ShaderParamDesc.h"


namespace bv {

class UniformShaderParam
{
private:

    char *                  m_buffer;
    ShaderParamDesc         m_desc;

private:

                                UniformShaderParam  ( const ShaderParamDesc & desc, char * pBuf );

public:

    void                        SetValue            ( const char * mem );

    const ShaderParamDesc &     Descriptor          () const;
    ParamType                   Type                () const;
    const std::string &         Name                () const;
    const char *                Data                () const;

    template< typename ValType >
    void                        GenericSetVal       ( const ValType * mem )
    {
        assert( sizeof( ValType ) == m_desc.Size() );
        SetValue( reinterpret_cast< const char * >( mem ) );
    }

    template< typename ValType >
    const ValType &             GenericGetVal       () const
    {
        assert( sizeof( ValType ) == m_desc.Size() );
        return *reinterpret_cast< const ValType * >( m_buffer );
    }

    friend class ShaderParamFactory;
};

// ******************************
class ShaderParamFloat : public UniformShaderParam
{
    void    Set    ( const float * mem );
};


// ******************************
class ShaderParamBool : public UniformShaderParam
{
    void    Set    ( const bool * mem );
};


// ******************************
class ShaderParamInt  : public UniformShaderParam
{
    void    Set    ( const int * mem );
};


class ShaderParamFactory
{
private:

    std::vector< char * >   m_ParamsData;

private:

    ShaderParamFactory()
    {
    }

    ~ShaderParamFactory();

public:

    UniformShaderParam * Create   ( const ShaderParamDesc & desc, char * pBuf );
    UniformShaderParam * Create   ( const ShaderParamDesc & desc );
    UniformShaderParam * Create   ( const std::string & name, ParamType type, char * pBuf = nullptr );

    static ShaderParamFactory & Get();

};

} //bv
