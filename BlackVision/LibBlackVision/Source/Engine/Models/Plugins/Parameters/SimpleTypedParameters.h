#pragma once

#include "Mathematics/Interpolators/Interpolators.h"
#include "Mathematics/Transform/MatTransform.h"

#include "Engine/Models/Plugins/Parameters/AbstractModelParameter.h"
#include "Engine/Interfaces/ISerializable.h"


namespace bv { namespace model {

// *******************************************
template< typename InterpolatorType, typename ValueType, ModelParamType type >
class SimpleParameterImpl : public AbstractModelParameter, public ISerializable
{
public:

    typedef ValueType ValType;

protected:

    InterpolatorType m_interpolator;

public:

    explicit                SimpleParameterImpl ( const std::string & name, const InterpolatorType & interpolator, ITimeEvaluatorPtr evaluator );
                            ~SimpleParameterImpl();

    virtual void                Serialize       ( SerializeObject &/*doc*/ ) const {}
    static ISerializablePtr     Create          ( DeserializeObject &/*doc*/ ) { return nullptr; }

    virtual void                SetInterpolationMethod ( InterpolationMethod method ) override;
    virtual InterpolationMethod GetInterpolationMethod () const override;

    inline  ValueType       Evaluate            () const;
    inline  void            SetVal              ( const ValueType & val, TimeType t );

    virtual VoidPtr         QueryParamTyped     () override;

    inline static  ModelParamType  Type         ();

};


// *******************************************
class ParamMat2 : public SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_MAT2 >
{
public:

    typedef glm::mat2 ValType;

private:

    typedef SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_MAT2 > Parent;

public:

    inline explicit         ParamMat2       ( const std::string & name, const Vec4Interpolator & transform, ITimeEvaluatorPtr evaluator );

    inline  glm::mat2       Evaluate        () const;
    inline  void            SetVal          ( const glm::mat2 & val, TimeType t );

    virtual VoidPtr         QueryParamTyped () override;

    inline static  ModelParamType  Type     ();

};

template<class T>
class ParamEnum : public SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM >
{
public:
    ParamEnum( const std::string & name, const IntInterpolator & interpolator, ITimeEvaluatorPtr evaluator );

    inline  T               Evaluate        () const;
    inline  void            SetVal          ( const T & val, TimeType t );

    virtual VoidPtr         QueryParamTyped () override;

    inline static  ModelParamType  Type     ();
};


typedef SimpleParameterImpl< FloatInterpolator, float, ModelParamType::MPT_FLOAT >      ParamFloat;
typedef SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_INT >            ParamInt;
typedef SimpleParameterImpl< BoolInterpolator, bool, ModelParamType::MPT_BOOL >         ParamBool;
typedef SimpleParameterImpl< Vec4Interpolator, glm::vec4, ModelParamType::MPT_VEC4 >    ParamVec4;
typedef SimpleParameterImpl< Vec3Interpolator, glm::vec3, ModelParamType::MPT_VEC3 >    ParamVec3;
typedef SimpleParameterImpl< Vec2Interpolator, glm::vec2, ModelParamType::MPT_VEC2 >    ParamVec2;

//template<typename T>
//using ParamEnumPtr<T> = std::shared_ptr<T>;

DEFINE_PTR_TYPE(ParamBool)
DEFINE_PTR_TYPE(ParamInt)
DEFINE_PTR_TYPE(ParamFloat)
DEFINE_PTR_TYPE(ParamVec4)
DEFINE_PTR_TYPE(ParamVec3)
DEFINE_PTR_TYPE(ParamVec2)
DEFINE_PTR_TYPE(ParamMat2)

} //model
} //bv

#include "SimpleTypedParameters.inl"
