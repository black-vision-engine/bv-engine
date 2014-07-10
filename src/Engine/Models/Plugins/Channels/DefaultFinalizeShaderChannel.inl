namespace bv { namespace model {

// *********************************
//
template< typename ShaderChannelIface >
DefaultFinalizeShaderChannel< ShaderChannelIface >::DefaultFinalizeShaderChannel    ( std::shared_ptr< ShaderChannelIface > channel )
    : m_channel( channel )
{
}

// *********************************
//
template< typename ShaderChannelIface >
DefaultFinalizeShaderChannel< ShaderChannelIface >::~DefaultFinalizeShaderChannel   ()
{
}

// *********************************
//
template< typename ShaderChannelIface >
bool    DefaultFinalizeShaderChannel< ShaderChannelIface >::IsReadOnly              () const    //Should also be in IChannel
{
    return m_channel->IsReadOnly();
}

// *********************************
//
template< typename ShaderChannelIface >
void    DefaultFinalizeShaderChannel< ShaderChannelIface >::PostUpdate              ()          //Should also be in IChannel
{
    m_channel->PostUpdate();
}

// *********************************
//
template< typename ShaderChannelIface >
const std::vector< bv::IValueConstPtr > &   DefaultFinalizeShaderChannel< ShaderChannelIface >::GetValues   () const
{
    return m_channel->GetValues();
}

// *********************************
//
template< typename ShaderChannelIface >
bv::IValueConstPtr  DefaultFinalizeShaderChannel< ShaderChannelIface >::GetValue            ( const std::string & name ) const
{
    return m_channel->GetValue( name );
}

// *********************************
//
template< typename ShaderChannelIface >
ITexturesDataConstPtr   DefaultFinalizeShaderChannel< ShaderChannelIface >::GetTexturesData () const
{
    return m_channel->GetTexturesData();
}

} //model
} //bv
