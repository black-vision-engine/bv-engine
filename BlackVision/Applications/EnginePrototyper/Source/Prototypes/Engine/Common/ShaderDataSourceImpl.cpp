#include "ShaderDataSourceImpl.h"

#include "Engine/Types/Values/ValuesFactory.h"


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
void                    ShaderDataSourceImpl::AddValue              ( const std::string & name, const glm::vec4 & val )
{
    if( GetValue( name ) == nullptr )
    {
        auto value = ValuesFactory::CreateValueVec4( name );
        value->SetValue( val );
        m_values.push_back( value );
    }
}

// ***************************
//
const std::string &     ShaderDataSourceImpl::GetShaderSource       ()  const
{
    return m_source;
}

// ***************************
//
ITexturesDataConstPtr   ShaderDataSourceImpl::GetTexturesData       ()  const
{
    return nullptr;
}

// ***************************
//
const std::vector< IValueConstPtr > &   ShaderDataSourceImpl::GetValues () const
{
    return m_values;
}

// ***************************
//
IValueConstPtr                          ShaderDataSourceImpl::GetValue  ( const std::string & name ) const
{
    for( IValueConstPtr val : m_values )
    {
        if( val->GetName() == name )
        {
            return val;
        }
    }

    return nullptr;
}

} // bv
