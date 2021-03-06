#pragma once

#include "Engine/Models/Plugins/Parameters/CachedSimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.inl"

#include "Mathematics/Interpolators/CompositeInterpolator.h"

namespace bv { namespace model {


enum GenericEnumType   : int    // bleee
{};

// *******************************
//
template< typename ValueType >
inline bool SetParameter( IParameterPtr parameter, TimeType t, const ValueType & val )
{
    static_assert( !"If this function doesn't compile - don't fix it. It means that compiler didn't choose proper template spezialization" );
    return false;
}


// *******************************
//
template< typename ValueType >
inline bool MoveParameterKey( IParameterPtr parameter, TimeType t, TimeType newTime )
{
    static_assert( !"If this function doesn't compile - don't fix it. It means that compiler didn't choose proper template spezialization" );
    return false;
}


bool    SetParameterRotation    ( IParameterPtr parameter, TimeType t, const glm::vec3 & eulerAngles );
bool    SetParameterScale       ( IParameterPtr parameter, TimeType t, const glm::vec3 & scale );
bool    SetParameterTranslation ( IParameterPtr parameter, TimeType t, const glm::vec3 & translation );
bool    SetParameterCenterMass  ( IParameterPtr parameter, TimeType t, const glm::vec3 & center );

bool    RemoveRotationKey       ( IParameterPtr parameter, TimeType t );
bool    RemoveScaleKey          ( IParameterPtr parameter, TimeType t );
bool    RemoveTranslationKey    ( IParameterPtr parameter, TimeType t );
bool    RemoveCenterMassKey     ( IParameterPtr parameter, TimeType t );

bool    MoveRotationKey         ( IParameterPtr parameter, TimeType t, TimeType newTime );
bool    MoveScaleKey            ( IParameterPtr parameter, TimeType t, TimeType newTime );
bool    MoveTranslationKey      ( IParameterPtr parameter, TimeType t, TimeType newTime );
bool    MoveCenterMassKey       ( IParameterPtr parameter, TimeType t, TimeType newTime );


// *******************************
//
bool                                                RemoveParameterKey ( IParameterPtr parameter, TimeType t );
bool                                                MoveParameterKey   ( IParameterPtr parameter, TimeType t, TimeType newTime );

bool                                                BezierSetGlobalCurveType ( IParameterPtr parameter, CurveType type );
bool                                                BezierSetAddedKeyCurveType ( IParameterPtr parameter, CurveType type );
CurveType                                           BezierGetCurveType ( IParameterPtr parameter );

bool                                                SetWrapPostMethod  ( IParameterPtr parameter, WrapMethod method );
bool                                                SetWrapPreMethod   ( IParameterPtr parameter, WrapMethod method );
WrapMethod                                          GetWrapPostMethod  ( IParameterPtr parameter );
WrapMethod                                          GetWrapPreMethod   ( IParameterPtr parameter );

int                                                 BezierParameterGetNumKeys( IParameterPtr parameter );

template< typename ValueType >
inline bool                                         BezierSetKey( IParameterPtr parameter, int key, const std::pair< ValueType, TimeType > & v1 )
{
    return false;
}

template< typename ValueType >
inline bool                                         BezierSetInterpolatorKey1( IParameterPtr parameter, int key, const std::pair< ValueType, TimeType > & v1 )
{
    return false;
}

template< typename ValueType >
inline bool                                         BezierSetInterpolatorKey2( IParameterPtr parameter, int key, const std::pair< ValueType, TimeType > & v2 )
{
    return false;
}

template< typename ValueType >
inline bool                                         BezierSetInterpolatorV1( IParameterPtr parameter, int key, const std::pair< ValueType, TimeType > & v1 )
{
    return false;
}

template< typename ValueType >
inline bool                                         BezierSetInterpolatorV2( IParameterPtr parameter, int key, const std::pair< ValueType, TimeType > & v2 )
{
    return false;
}

std::string                                         EvaluateParamToString( IParameterPtr parameter, TimeType t );

} //model
} //bv



#define DEFINE_ENUM_SET_PARAMETER( enumName )                                                               \
template<>                                                                                                  \
inline bool ::bv::model::SetParameter< enumName >( ::bv::model::IParameterPtr param, TimeType t, const enumName & val )    \
{                                                                                                           \
    typedef ::bv::model::ParamEnum< enumName > ParamType;                                                     \
                                                                                                            \
    ParamType * typedParam = ::bv::model::QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();   \
    if( typedParam == nullptr )                                                                             \
    {                                                                                                       \
        return false;                                                                                       \
    }                                                                                                       \
    typedParam->SetVal( val, t );                                                                           \
    return true;                                                                                            \
}



#include "GenericParameterSetters.inl"
