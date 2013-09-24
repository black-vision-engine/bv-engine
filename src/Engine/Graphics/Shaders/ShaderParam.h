#pragma once

#include "Engine/Types/Enums.h"

#include "ShaderParamDesc.h"
#include <cassert>
#include <vector>

namespace bv {

class IShaderParamUpdater;

class GenericShaderParam
{
private:

    char *                  m_buffer;
    ShaderParamDesc         m_desc;
    IShaderParamUpdater *   m_updater;

private:
                                GenericShaderParam  ( const ShaderParamDesc & desc, char * pBuf );
public:

    void                        RegisterUpdater     ( IShaderParamUpdater * updater );
    void                        Update              ();

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
class ShaderParamFloat : public GenericShaderParam
{
    void    Set    ( const float * mem );
};


// ******************************
class ShaderParamBool : public GenericShaderParam
{
    void    Set    ( const bool * mem );
};


// ******************************
class ShaderParamInt  : public GenericShaderParam
{
    void    Set    ( const int * mem );
};


class ShaderParamFactory
{
private:
    std::vector<char*> m_ParamsData;

    ShaderParamFactory(){}
    ~ShaderParamFactory();

public:
    GenericShaderParam* Create   ( const ShaderParamDesc & desc, char * pBuf );
    GenericShaderParam* Create   ( const ShaderParamDesc & desc );
    GenericShaderParam* Create   ( const std::string& name, ParamType type, char* pBuf = nullptr);

    static ShaderParamFactory& Get();
};

} //bv
