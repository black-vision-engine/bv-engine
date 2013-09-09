#include "ShaderParam.h"
#include "ShaderParamUpdater.h"

#include <cassert>

namespace bv {

// ***********************
//
GenericShaderParam::GenericShaderParam  ( const ShaderParamDesc & desc, char * pMem )
    : m_buffer( pMem )
    , m_desc( desc )
    , m_updater( nullptr )
{
    assert( pMem );
}

// ***********************
//
void                        GenericShaderParam::RegisterUpdater ( IShaderParamUpdater * updater )
{
    m_updater = updater;
}

// ***********************
//
void                        GenericShaderParam::Update              ()
{
    m_updater->Update( this );
}

// ***********************
//
void                        GenericShaderParam::SetValue    ( const char * mem )
{
    memcpy( m_buffer, mem, m_desc.Size() );
}

// ***********************
//
const ShaderParamDesc &     GenericShaderParam::Descriptor  () const
{
    return m_desc;
}

// ***********************
//
ParamType  GenericShaderParam::Type        () const
{
    return m_desc.Type();
}

// ***********************
//
const std::string &         GenericShaderParam::Name         () const
{
    return m_desc.Name();
}

// ***********************
//
const char *                GenericShaderParam::Data        () const
{
    return m_buffer;
}

// ***********************
//
void    ShaderParamFloat::Set    ( const float * mem )
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
void    ShaderParamInt::Set    ( const int * mem )
{
    GenericSetVal< int >( mem );
}

// ***********************
//
GenericShaderParam* ShaderParamFactory::Create   ( const ShaderParamDesc & desc, char * pBuf )
{
    return new GenericShaderParam(desc, pBuf);
}

// ***********************
//
GenericShaderParam* ShaderParamFactory::Create   ( const ShaderParamDesc & desc )
{
    char* data = new char[desc.Size()];
    m_ParamsData.push_back(data);
    return Create(desc, data);
}

// ***********************
//
GenericShaderParam* ShaderParamFactory::Create   ( const std::string& name, ParamType type, char* pBuf)
{
    ShaderParamDesc desc = ShaderParamDesc::Create(name, type);

    if(pBuf)
    {
        return Create(desc, pBuf);
    }
    else
    {
        return Create(desc);
    }
}

ShaderParamFactory& ShaderParamFactory::Get()
{
    static ShaderParamFactory instance;
    return instance;
}

ShaderParamFactory::~ShaderParamFactory()
{
    for(auto p : m_ParamsData)
    {
        delete[] p;
    }
}

} //bv
