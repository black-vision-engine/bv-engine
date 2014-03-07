namespace bv {

// ****************************************************** ShaderTraits specializations ******************************************************

SPECIALIZE_SHADER_TRAIT( ParamType::PT_FLOAT, float );
SPECIALIZE_SHADER_TRAIT( ParamType::PT_FLOAT2, glm::vec2 );
SPECIALIZE_SHADER_TRAIT( ParamType::PT_FLOAT3, glm::vec3 );
SPECIALIZE_SHADER_TRAIT( ParamType::PT_FLOAT4, glm::vec4 );
SPECIALIZE_SHADER_TRAIT( ParamType::PT_MAT2, glm::mat2 );
SPECIALIZE_SHADER_TRAIT( ParamType::PT_MAT4, glm::mat4 );


// *********************************************************** Inline implementations **********************************************************

// ****************************
//
template< typename ValType >
const ValType &             GenericShaderParam::GenericGetValue () const
{
    assert( ShaderParamTypeTraits< ValType >::paramType = m_paramType );

    const void * rawVal = GetValuePtr();
    assert( rawVal != nullptr );

    return *reinterpret_cast< const ValType * >( rawVal );
}

// ****************************
//
inline ParamType            GenericShaderParam::Type            () const
{
    return m_paramType;
}

// ****************************
//
inline const std::string &  GenericShaderParam::Name            () const
{
    return m_paramName;
}


} //bv
