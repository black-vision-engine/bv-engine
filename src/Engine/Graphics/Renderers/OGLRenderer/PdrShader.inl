namespace bv
{

// *******************************
//FIXME: reimplement this method so that switch-case statement is not called here but rather in GenericParameter or even better - in shader desc
inline  void    PdrShader::SetUniformParam ( const GenericShaderParam * param )
{
    switch( param->Type() )
    {
        case ParamType::PT_FLOAT1:
            SetUniform< ParamType::PT_FLOAT1 >( param );
            break;
        case ParamType::PT_FLOAT2:
            SetUniform< ParamType::PT_FLOAT2 >( param );
            break;
        case ParamType::PT_FLOAT3:
            SetUniform< ParamType::PT_FLOAT3 >( param );
            break;
        case ParamType::PT_FLOAT4:
            SetUniform< ParamType::PT_FLOAT4 >( param );
            break;
        case ParamType::PT_MAT2:
            SetUniform< ParamType::PT_MAT2 >( param );
            break;
        case ParamType::PT_MAT3:
            SetUniform< ParamType::PT_MAT3 >( param );
            break;
        case ParamType::PT_MAT4:
            SetUniform< ParamType::PT_MAT4 >( param );
            break;
        case ParamType::PT_INT:
            SetUniform< ParamType::PT_INT >( param );
            break;
        case ParamType::PT_BOOL:
            SetUniform< ParamType::PT_BOOL >( param );
            break;
        default:
            assert( false );
            break;
    }
}

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
    m_program->SetUniform( param->IntID(), val );
}

} //bv
