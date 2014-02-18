namespace bv { namespace model {

// *********************************
//
template< typename ShaderChannelIface >
ShaderChannel< ShaderChannelIface >::ShaderChannel  ( const std::string & shaderFile, std::vector< IValue * > * values )
    : m_values( values )
{
    assert( values != nullptr );

    std::stringstream shaderSource;

    File::Open( shaderFile ) >> shaderSource;

    m_shaderSource = shaderSource.str(); 
}

// *********************************
//
template< typename ShaderChannelIface >
const std::string &         ShaderChannel< ShaderChannelIface >::GetShaderSource  () const
{
    return m_shaderSource; 
}

// *********************************
//
template< typename ShaderChannelIface >
bool                        ShaderChannel< ShaderChannelIface >::IsReadOnly       () const
{
    return true; 
}

// *********************************
//
template< typename ShaderChannelIface >
void                        ShaderChannel< ShaderChannelIface >::PostUpdate       ()
{
    //Default implementation is empty
}

// *********************************
//
template< typename ShaderChannelIface >
std::vector< IValue* > &    ShaderChannel< ShaderChannelIface >::GetValues        () 
{
    return *m_values; 
}

// *********************************
//
template< typename ShaderChannelIface >
IValue *                    ShaderChannel< ShaderChannelIface >::GetValue           ( const std::string & name )
{
    for( auto value : *m_values )
    {
        if( value->GetName() == name )
        {
            return value;
        }
    }
}

} // model
} // bv
