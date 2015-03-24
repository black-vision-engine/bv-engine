#pragma once
#include "Engine/Models/Plugins/Interfaces/ICachedParameter.h"
#include "SimpleTypedParameters.h"

namespace bv { namespace model {

namespace CachedSimpleTypedParametersHelper
{
    const float feps = 0.0000001f; // FIXME: make this non-const?

    template< typename T > 
        static inline bool isDifferent( T& a, T& b );
}

// *******************************************
template< typename InterpolatorType, typename ValueType, ModelParamType type >
class CachedSimpleTypedParameters : 
    public SimpleParameterImpl< InterpolatorType, ValueType, type >
    , public ICachedParameter
{
    typedef SimpleParameterImpl< InterpolatorType, ValueType, type > ParentImpl;

    mutable bool                    changed;

    mutable ValueType               curValue;
    mutable ValueType               prevValue;

    //template< typename T > 
    //    static inline bool isDifferent( T& a, T& b );
public:
    CachedSimpleTypedParameters( const std::string & name, const InterpolatorType & interpolator, ITimeEvaluatorPtr evaluator );

    inline  ValueType               Evaluate            () const;
    virtual bool                    Changed             () const override;
};

template<class T>
class ParamEnum : public CachedSimpleTypedParameters< IntInterpolator, int, ModelParamType::MPT_ENUM >/*, public ICachedParameter*/
{
    typedef CachedSimpleTypedParameters< IntInterpolator, int, ModelParamType::MPT_ENUM > ParentImpl;
public:
    ParamEnum( const std::string & name, const IntInterpolator & interpolator, ITimeEvaluatorPtr evaluator );

    inline  T               Evaluate        () const;
    inline  void            SetVal          ( const T & val, TimeType t );

    virtual VoidPtr         QueryParamTyped () override;

    inline static  ModelParamType  Type     ();
    //virtual bool                    Changed             () const override { return ParentImpl::Changed(); }
};



//typedef CachedSimpleTypedParameters< FloatInterpolator, float, ModelParamType::MPT_FLOAT >      ParamFloat;
//typedef CachedSimpleTypedParameters< IntInterpolator, int, ModelParamType::MPT_INT >            ParamInt;
//typedef CachedSimpleTypedParameters< IntInterpolator, int, ModelParamType::MPT_ENUM >           ParamEnumParent;
//typedef CachedSimpleTypedParameters< BoolInterpolator, bool, ModelParamType::MPT_BOOL >         ParamBool;
//typedef CachedSimpleTypedParameters< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4 >    ParamVec4;
//typedef CachedSimpleTypedParameters< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3 >    ParamVec3;
//typedef CachedSimpleTypedParameters< Vec2Interpolator, glm::vec2, ModelParamType::MPT_VEC2 >    ParamVec2;
//
////template<typename T>
////using ParamEnumPtr<T> = std::shared_ptr<T>;
//
//DEFINE_PTR_TYPE(ParamBool)
//DEFINE_PTR_TYPE(ParamInt)
//DEFINE_PTR_TYPE(ParamFloat)
//DEFINE_PTR_TYPE(ParamVec4)
//DEFINE_PTR_TYPE(ParamVec3)
//DEFINE_PTR_TYPE(ParamVec2)
//DEFINE_PTR_TYPE(ParamMat2)

} //model
} //bv

#include "CachedSimpleTypedParameters.inl"
