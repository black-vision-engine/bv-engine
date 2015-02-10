#include "ShaderDataSourceImpl.h"

namespace bv {

// ***************************
//
ShaderDataSourceImpl::ShaderDataSourceImpl( const std::string & source )
    : m_source( source )
{
}

// ***************************
//
ShaderDataSourceImpl::~ShaderDataSourceImpl   ()
{
}

// ***************************
//
const std::string &     ShaderDataSourceImpl::GetShaderSource     ()  const
{
    return m_source;
}

// ***************************
//
ITexturesDataConstPtr   ShaderDataSourceImpl::GetTexturesData     ()  const
{
    return nullptr;
}

// ***************************
//
const std::vector< IValueConstPtr > &   ShaderDataSourceImpl::GetValues   () const
{
    // FIXME: implement
    static std::vector< IValueConstPtr > dummy;

    return dummy;
}

// ***************************
//
IValueConstPtr                          ShaderDataSourceImpl::GetValue    ( const std::string & name ) const
{
    // FIXME: implement
    { name; }
    return nullptr;
}

} // bv
