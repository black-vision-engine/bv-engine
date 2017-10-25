#pragma once

#include "ConstFunction.h"
#include "LinearFunction.h"
#include "BezierFunction.h"
#include "PolynomialFunction.h"




namespace bv
{

// ========================================================================= //
// Is valid evaluator trait
// ========================================================================= //



// ***********************
// Enables all evaluators. Spetialize this trait for types that should have other behavior.
template< typename ValueT, EvaluatorType EvalType >
struct IsValidEvaluatorTrait
{
    static const bool Value = true;
};


// ***********************
// Call this function if you don't like syntax IsValidEvaluator< bla, bla >::Value.
template< typename ValueT, EvaluatorType EvalType >
inline bool IsValidEvaluator()
{
    return IsValidEvaluatorTrait< ValueT, EvalType >::Value;
}


// ***********************
//
#define DISABLE_EVALUATOR_FOR_TYPE( typeName, evalType )                \
template<>                                                              \
struct IsValidEvaluatorTrait< typeName, evalType >                      \
{                                                                       \
    static const bool Value = false;                                    \
};

// ***********************
//
#define ENABLE_EVALUATOR_FOR_TYPE( typeName, evalType )                 \
template<>                                                              \
struct IsValidEvaluatorTrait< typeName, evalType >                      \
{                                                                       \
    static const bool Value = true;                                     \
};


// ***********************
// Partialy specialize template to disable all evaluators. Then you can use ENABLE_EVALUATOR_FOR_TYPE.
#define DISABLE_ALL_EVALUATORS_FOR_TYPE( typeName )                     \
template< EvaluatorType EvalType >                                      \
struct IsValidEvaluatorTrait< typeName, EvalType >                      \
{                                                                       \
    static const bool Value = false;                                    \
};


// ***********************
// std::string
DISABLE_ALL_EVALUATORS_FOR_TYPE( std::string )
ENABLE_EVALUATOR_FOR_TYPE( std::string, EvaluatorType::ET_CONSTANT )

// ***********************
// std::wstring
DISABLE_ALL_EVALUATORS_FOR_TYPE( std::wstring )
ENABLE_EVALUATOR_FOR_TYPE( std::wstring, EvaluatorType::ET_CONSTANT )


// ========================================================================= //
// Type and curve validation functions
// ========================================================================= //


// ***********************
//
template< typename ValueT >
inline bool                     IsValidEvaluatorType    ( EvaluatorType type )
{
    switch( type )
    {
    case EvaluatorType::ET_CONSTANT:
        return IsValidEvaluator< ValueT, EvaluatorType::ET_CONSTANT >();
    case EvaluatorType::ET_LINEAR:
        return IsValidEvaluator< ValueT, EvaluatorType::ET_LINEAR >();
    case EvaluatorType::ET_BEZIER:
        return IsValidEvaluator< ValueT, EvaluatorType::ET_BEZIER >();
    case EvaluatorType::ET_POLYNOMIAL:
        return IsValidEvaluator< ValueT, EvaluatorType::ET_POLYNOMIAL >();
    default:
        assert( !"Why this function isn't implemented for this EvaluatorType?" );
        break;
    }

    return false;
}


// ***********************
//
template< typename ValueT >
inline EvaluatorType            GetEvaluatorType        ( CurveType curve )
{
    switch( curve )
    {
    case CurveType::CT_POINT:
        return EvaluatorType::ET_CONSTANT;
    case CurveType::CT_LINEAR:
        return EvaluatorType::ET_LINEAR;
    case CurveType::CT_BEZIER:
    case CurveType::CT_COSINE_LIKE:
        return EvaluatorType::ET_BEZIER;
    case CurveType::CT_CUBIC_IN:
    case CurveType::CT_CUBIC_OUT:
    case CurveType::CT_ELASTIC_IN:
    case CurveType::CT_ELASTIC_OUT:
    case CurveType::CT_ELASTIC_IN_BOUNCE:
    case CurveType::CT_ELASTIC_OUT_BOUNCE:
    case CurveType::CT_QUARTIC_INOUT:
    case CurveType::CT_CUBIC_INTOUT:
        return EvaluatorType::ET_POLYNOMIAL;
    default:
        assert( !"Why this function isn't implemented for this CurveType?" );
        return EvaluatorType::ET_CONSTANT;  // It's the safest way for release build since everyone should implement this.
    }
}

// ***********************
//
template< typename ValueT >
inline bool                 IsValidCurveType            ( CurveType curve )
{
    EvaluatorType evalType = GetEvaluatorType( curvet );
    return IsValidEvaluatorType< ValueT >( evalType );
}


}	// bv

