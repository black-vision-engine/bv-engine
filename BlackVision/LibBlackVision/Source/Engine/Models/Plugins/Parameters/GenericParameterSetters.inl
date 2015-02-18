namespace bv { namespace model {

namespace {

// *******************************
//
template< typename ParamType >
inline bool SetSimpleTypedParameter( IParameterPtr param, TimeType t, typename const ParamType::ValType & val )
{
    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

} //anonymous


// *******************************
//
template<>
inline bool SetParameter< int >( IParameterPtr param, TimeType t, const int & val )
{
    return SetSimpleTypedParameter< ParamInt >( param, t, val );
}

// *******************************
//
template<>
inline bool SetParameter< float >( IParameterPtr param, TimeType t, const float & val )
{
    return SetSimpleTypedParameter< ParamFloat >( param, t, val );
}

// *******************************
//
template<>
inline bool SetParameter< glm::vec2 >( IParameterPtr param, TimeType t, const glm::vec2 & val )
{
    return SetSimpleTypedParameter< ParamVec2 >( param, t, val );
}

// *******************************
//
template<>
inline bool SetParameter< glm::vec3 >( IParameterPtr param, TimeType t, const glm::vec3 & val )
{
    return SetSimpleTypedParameter< ParamVec3 >( param, t, val );
}

// *******************************
//
template<>
inline bool SetParameter< glm::vec4 >( IParameterPtr param, TimeType t, const glm::vec4 & val )
{
    return SetSimpleTypedParameter< ParamVec4 >( param, t, val );
}

// *******************************
//
template<>
inline bool SetParameter< glm::mat2 >( IParameterPtr param, TimeType t, const glm::mat2 & val )
{
    return SetSimpleTypedParameter< ParamMat2 >( param, t, val );
}

} //model
} //bv
