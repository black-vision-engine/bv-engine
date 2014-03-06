#include "ShaderParam.h"

#include <cassert>

#include "Engine/Graphics/Shaders/IShaderParamUpdater.h"


namespace bv {

// ***********************
//
UniformShaderParam::UniformShaderParam  ( const ShaderParamDesc & desc, char * pMem )
    : m_buffer( pMem )
    , m_desc( desc )
{
    assert( pMem );
}

// ***********************
//
void                        UniformShaderParam::SetValue        ( const char * mem )
{
    memcpy( m_buffer, mem, m_desc.Size() );
}

// ***********************
//
const ShaderParamDesc &     UniformShaderParam::Descriptor      () const
{
    return m_desc;
}

// ***********************
//
ParamType  UniformShaderParam::Type                     () const
{
    return m_desc.Type();
}

// ***********************
//
const std::string &         UniformShaderParam::Name    () const
{
    return m_desc.Name();
}

// ***********************
//
const char *                UniformShaderParam::Data    () const
{
    return m_buffer;
}

// ***********************
//
void    ShaderParamFloat::Set   ( const float * mem )
{
    GenericSetVal< float >( mem );
}

// ***********************
//
void    ShaderParamBool::Set    ( const bool * mem )
{
    GenericSetVal< bool >( mem );
}

// ***********************
//
void    ShaderParamInt::Set     ( const int * mem )
{
    GenericSetVal< int >( mem );
}

// ***********************
//
UniformShaderParam * ShaderParamFactory::Create   ( const ShaderParamDesc & desc, char * pBuf )
{
    return new UniformShaderParam( desc, pBuf );
}

// ***********************
//
UniformShaderParam * ShaderParamFactory::Create   ( const ShaderParamDesc & desc )
{
    char * data = new char[ desc.Size() ];
    m_ParamsData.push_back( data );
    return Create( desc, data );
}

// ***********************
//
UniformShaderParam * ShaderParamFactory::Create   ( const std::string & name, ParamType type, char * pBuf )
{
    ShaderParamDesc desc = ShaderParamDesc::Create( name, type );

    if( pBuf )
    {
        return Create( desc, pBuf );
    }
    else
    {
        return Create( desc );
    }
}

// ***********************
//
ShaderParamFactory & ShaderParamFactory::Get()
{
    static ShaderParamFactory instance;
    return instance;
}

// ***********************
//
ShaderParamFactory::~ShaderParamFactory()
{
    for( auto p : m_ParamsData )
    {
        delete[] p;
    }
}

} //bv
