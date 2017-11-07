#pragma once

#include "Engine/Models/Timeline/TimelineManager.h"
#include "Serialization/SerializationHelper.h"
#include "Serialization/BV/BVSerializeContext.h"

namespace bv { namespace model {

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
SimpleParameterImpl< InterpolatorType, ValueType, type >::SimpleParameterImpl( const std::string & name, const InterpolatorType & interpolator, ITimeEvaluatorPtr evaluator )
    : AbstractModelParameter( name, type, evaluator )
    , m_interpolator( interpolator )
{
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
SimpleParameterImpl< InterpolatorType, ValueType, type >::~SimpleParameterImpl    ()
{
}

// ********************************************************************************************************************

std::string Type2String( ModelParamType type ); // FIXME

template< typename InterpolatorType, typename ValueType, ModelParamType type >
void                SimpleParameterImpl< InterpolatorType, ValueType, type >::Serialize       ( ISerializer& ser ) const
{
    auto serContext = Cast< BVSerializeContext * >( ser.GetSerializeContext() );

    ser.EnterChild( "param" );
    ser.SetAttribute( "name", GetName() );
    ser.SetAttribute( "type", Convert::T2String< ModelParamType >( GetType() ) );
    
    std::string timeline;
    if( serContext->sceneNameInTimeline )
        timeline = serContext->GetTimelineManager()->GetTimelinePath( m_timeEvaluator );
    else
        timeline = m_timeEvaluator->GetName();
    ser.SetAttribute( "timeline", timeline );

    m_interpolator.Serialize( ser );

    ser.ExitChild();
}

// ***********************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
void            SimpleParameterImpl< InterpolatorType, ValueType, type >::SetAddedKeyCurveType    ( CurveType type )
{
    m_interpolator.SetAddedKeyCurveType( type ); 
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
void SimpleParameterImpl< InterpolatorType, ValueType, type >::SetGlobalCurveType    ( CurveType type )
{ 
    m_interpolator.SetGlobalCurveType( type ); 
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
CurveType       SimpleParameterImpl< InterpolatorType, ValueType, type >::GetCurveType        ()
{
    return m_interpolator.GetCurveType();
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
void                SimpleParameterImpl< InterpolatorType, ValueType, type >::SetWrapPostMethod       ( WrapMethod method )
{
    m_interpolator.SetWrapPostMethod( method );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
void                SimpleParameterImpl< InterpolatorType, ValueType, type >::SetWrapPreMethod        ( WrapMethod method )
{
    m_interpolator.SetWrapPreMethod( method );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
WrapMethod          SimpleParameterImpl< InterpolatorType, ValueType, type >::GetWrapPostMethod       ()
{
    return m_interpolator.GetWrapPostMethod();
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
WrapMethod          SimpleParameterImpl< InterpolatorType, ValueType, type >::GetWrapPreMethod        ()
{
    return m_interpolator.GetWrapPreMethod();
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
int                 SimpleParameterImpl< InterpolatorType, ValueType, type >::GetNumKeys              ()
{
    return m_interpolator.GetNumKeys();
}


// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline ValueType SimpleParameterImpl< InterpolatorType, ValueType, type >::Evaluate () const
{
    auto t = GetLocalEvaluationTime();

    return m_interpolator.Evaluate( t );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline  std::string         SimpleParameterImpl< InterpolatorType, ValueType, type >::EvaluateToString        ( TimeType t ) const
{
    auto val = m_interpolator.Evaluate( t );

    return Convert::T2String( val );
}


// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline  void        SimpleParameterImpl< InterpolatorType, ValueType, type >::SetVal    ( const ValueType & val, TimeType t )
{
    m_interpolator.AddKey( t, val );
}

// ***********************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline  bool        SimpleParameterImpl< InterpolatorType, ValueType, type >::RemoveVal ( TimeType t )
{
    return m_interpolator.RemoveKey( t );
}

// ***********************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline  bool        SimpleParameterImpl< InterpolatorType, ValueType, type >::MoveKey             ( TimeType t, TimeType newTime )
{
    return m_interpolator.MoveKey( t, newTime );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
InterpolatorType &		SimpleParameterImpl< InterpolatorType, ValueType, type >::AccessInterpolator	()
{
    return m_interpolator;
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
VoidPtr     SimpleParameterImpl< InterpolatorType, ValueType, type >::QueryParamTyped  ()
{
    return std::static_pointer_cast< void >( shared_from_this() );
}

// *******************************
//
template< typename InterpolatorType, typename ValueType, ModelParamType type >
inline ModelParamType  SimpleParameterImpl< InterpolatorType, ValueType, type >::Type   ()
{
    return type;
}

// *******************************
//
inline  ParamMat2::ParamMat2( const std::string & name, const Vec4Interpolator & transform, ITimeEvaluatorPtr evaluator )
    : Parent( name, transform, evaluator )
{
}

// *******************************
//
inline  glm::mat2   ParamMat2::Evaluate    () const
{
    glm::vec4 v = Parent::Evaluate();

    return glm::mat2( v[ 0 ], v[ 1 ], v[ 2 ], v [ 3 ] );
}

// *******************************
//
inline  void        ParamMat2::SetVal    ( const glm::mat2 & val, TimeType t )
{
    m_interpolator.AddKey( t, glm::vec4( val[ 0 ][ 0 ], val[ 1 ][ 0 ], val[ 1 ][ 0 ], val[ 1 ][ 1 ] ) );
}

// *******************************
//
inline ModelParamType  ParamMat2::Type     ()
{
    return ModelParamType::MPT_MAT2;
}

//// *******************************
////
//template<class T>
//inline  ParamEnum<T>::ParamEnum( const std::string & name, const IntInterpolator & transform, ITimeEvaluatorPtr evaluator )
//    : SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM >( name, transform, evaluator )
//{
//}
//
//// *******************************
////
//template<class T>
//inline  T   ParamEnum<T>::Evaluate    () const
//{
//    int v = SimpleParameterImpl< IntInterpolator, int, ModelParamType::MPT_ENUM >::Evaluate();
//
//    return T(v);
//}
//
//// *******************************
////
//template<class T>
//inline  void        ParamEnum<T>::SetVal    ( const T & val, TimeType t )
//{
//    m_interpolator.AddKey( t, int(val) );
//}
//
//// *******************************
////
//template<class T>
//inline ModelParamType  ParamEnum<T>::Type     ()
//{
//    return ModelParamType::MPT_ENUM;
//}

} //model
} //bv
