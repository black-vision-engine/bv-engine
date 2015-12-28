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

template<>
inline bool SetSimpleTypedParameter< ParamEnum< GenericEnumType > >( IParameterPtr param, TimeType t, const ParamEnum<GenericEnumType>::ValType & val )
{
    ParamEnum< GenericEnumType > * typedParam = QueryTypedParam< std::shared_ptr< ParamEnum< GenericEnumType > > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( static_cast< GenericEnumType >( val ), t );

    return true;
}


template<ModelParamType ParamType>
class ParamTypeTrait
{ public: typedef ParamBool ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_BOOL>
{ public: typedef ParamBool ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_ENUM>
{ public: typedef ParamEnum<GenericEnumType> ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_FLOAT>
{ public: typedef ParamFloat ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_INT>
{ public: typedef ParamInt ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_MAT2>
{ public: typedef ParamMat2 ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_STRING>
{ public: typedef ParamString ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_VEC2>
{ public: typedef ParamVec2 ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_VEC3>
{ public: typedef ParamVec3 ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_VEC4>
{ public: typedef ParamVec4 ObjectType; };

template<>
class ParamTypeTrait<ModelParamType::MPT_WSTRING>
{ public: typedef ParamWString ObjectType; };


template<ModelParamType ParamType>
bool RemoveTypedParamKey( IParameterPtr parameter, TimeType t )
{
    typedef ParamTypeTrait<ParamType>::ObjectType CastType;

    CastType* typedParam = QueryTypedParam< std::shared_ptr< CastType > >( parameter ).get();
    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->RemoveVal( t );

    return true;
}

} //anonymous


// *******************************
//
template<>
inline bool SetParameter< GenericEnumType >( IParameterPtr param, TimeType t, const GenericEnumType & val )
{
    return SetSimpleTypedParameter< ParamEnum<GenericEnumType> >( param, t, int( val ) );
}

// *******************************
//
template<>
inline bool SetParameter< bool >( IParameterPtr param, TimeType t, const bool & val )
{
    return SetSimpleTypedParameter< ParamBool >( param, t, val );
}

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

// *******************************
//
template<>
inline bool SetParameter< std::wstring >( IParameterPtr param, TimeType t, const std::wstring & val )
{
    return SetSimpleTypedParameter< ParamWString >( param, t, val );
}

// *******************************
//
template<>
inline bool SetParameter< std::string >( IParameterPtr param, TimeType t, const std::string & val )
{
    return SetSimpleTypedParameter< ParamString >( param, t, val );
}

//// *******************************
////
//template<> template< typename T >
//inline bool SetParameter< T >( IParameterPtr param, TimeType t, const T & val )
//{
//    return SetSimpleTypedParameter< ParamEnum<T> >( param, t, val );
//}


} //model
} //bv
