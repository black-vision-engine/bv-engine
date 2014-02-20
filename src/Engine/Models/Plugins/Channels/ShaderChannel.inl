namespace bv { namespace model {

// *********************************
//
template< typename ShaderChannelIface >
ShaderChannel< ShaderChannelIface >::ShaderChannel  ( const std::string & shaderSource, const IValueSet * valueSet )
    : m_values( valueSet )
    , m_shaderSource( shaderSource )
{
    assert( valueSet != nullptr );
}


// *********************************
//
template< typename ShaderChannelIface >
const std::string &         ShaderChannel< ShaderChannelIface >::GetShaderSource    () const
{
    return m_shaderSource; 
}

// *********************************
//
template< typename ShaderChannelIface >
bool                        ShaderChannel< ShaderChannelIface >::IsReadOnly         () const
{
    return true; 
}

// *********************************
//
template< typename ShaderChannelIface >
void                        ShaderChannel< ShaderChannelIface >::PostUpdate         ()
{
    //Default implementation is empty
}

// *********************************
//
template< typename ShaderChannelIface >
const std::vector< IValue* > &  ShaderChannel< ShaderChannelIface >::GetValues      () const
{
    return m_values->GetValues(); 
}

// *********************************
//
template< typename ShaderChannelIface >
IValue *                        ShaderChannel< ShaderChannelIface >::GetValue       ( const std::string & name ) const
{
    for( auto value : m_values->GetValues() )
    {
        if( value->GetName() == name )
        {
            return value;
        }
    }

    return nullptr;
}

} // model
} // bv
