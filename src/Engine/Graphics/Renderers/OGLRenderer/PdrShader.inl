namespace bv
{

// *******************************
//
template< ParamType paramType >
inline void PdrShader::InitSetUniform   ( const GenericShaderParam * param )
{
    typedef ShaderValueTypeTraits< paramType >::ValueType ValueType;

    const ValueType & val = param->GenericGetValue< ValueType >();

    int loc = m_program->SetUniform( param->Name().c_str(), val );

    param->SetIntID( loc );

    //FIXME: make sure that this is the right way to do it (should be, but better be sure)
    if( loc < 0 )
        param->SetUpdatable( false );
}

// *******************************
//
template< ParamType paramType >
inline void PdrShader::SetUniform       ( const GenericShaderParam * param )
{
    typedef ShaderValueTypeTraits< paramType >::ValueType ValueType;
    
    const ValueType & val = param->GenericGetValue< ValueType >();
    int loc = param->IntID();

    //FIXME: maybe this is not necessary, but for the time being may stay here (until active uniforms set is added or something similar is implemented)
    if( loc > 0 )
        m_program->SetUniform( param->IntID(), val );
}

} //bv
