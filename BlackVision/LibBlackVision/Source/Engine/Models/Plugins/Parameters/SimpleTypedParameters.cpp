#include "stdafx.h"

#include "SimpleTypedParameters.h"
#include "SimpleTypedParameters.inl"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

std::string Type2String( ModelParamType type )
{
    if( type == ModelParamType::MPT_FLOAT )
        return "float";

    if( type == ModelParamType::MPT_STRING )
        return "string";

    if( type == ModelParamType::MPT_WSTRING )
        return "wstring";

    //assert( false );
    return std::to_string( int ( type ) );
}

// *******************************
//
VoidPtr ParamMat2::QueryParamTyped ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

template class SimpleParameterImpl< StringInterpolator, std::string, ModelParamType::MPT_STRING >;
template class SimpleParameterImpl< FloatInterpolator, float, ModelParamType::MPT_FLOAT >;
template class SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_INT >;
template class SimpleParameterImpl< BoolInterpolator, bool, ModelParamType::MPT_BOOL >;
template class SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4 >;
template class SimpleParameterImpl< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3 >;
template class SimpleParameterImpl< Vec2Interpolator, glm::vec2, ModelParamType::MPT_VEC2 >;
template class SimpleParameterImpl< StringInterpolator, std::string, ModelParamType::MPT_STRING >;
template class SimpleParameterImpl< WStringInterpolator, std::wstring, ModelParamType::MPT_WSTRING >;
template class SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM >;

template class SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_MAT2 >;

} //model
} //bv
