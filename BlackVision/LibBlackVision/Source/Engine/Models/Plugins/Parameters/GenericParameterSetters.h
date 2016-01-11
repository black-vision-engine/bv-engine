#pragma once

#include "Engine/Models/Plugins/Parameters/CachedSimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"

#include "Mathematics/Interpolators/CompositeBezierInterpolator.h"

namespace bv { namespace model {


enum GenericEnumType   : int    // bleee
{};

// *******************************
//
template< typename ValueType >
inline bool SetParameter( IParameterPtr parameter, TimeType t, const ValueType & val )
{
    assert( !"If this function doesn't compile - don't fix it. It means that compiler didn't choose proper template spezialization" );
    return false;
}

bool    SetParameterRotation    ( IParameterPtr parameter, TimeType t, const glm::vec3 & rotAxis, float angle );
bool    SetParameterScale       ( IParameterPtr parameter, TimeType t, const glm::vec3 & scale );
bool    SetParameterTranslation ( IParameterPtr parameter, TimeType t, const glm::vec3 & translation );
bool    SetParameterCenterMass  ( IParameterPtr parameter, TimeType t, const glm::vec3 & center );

bool    SetParameterRotation    ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & rotAxis, float angle );
bool    SetParameterScale       ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & scale );
bool    SetParameterTranslation ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & translation );
bool    SetParameterCenterMass  ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & center );

bool    RemoveRotationKey       ( IParameterPtr parameter, unsigned int idx, TimeType t );
bool    RemoveScaleKey          ( IParameterPtr parameter, unsigned int idx, TimeType t );
bool    RemoveTranslationKey    ( IParameterPtr parameter, unsigned int idx, TimeType t );
bool    RemoveCenterMassKey     ( IParameterPtr parameter, unsigned int idx, TimeType t );

bool    RemoveRotationKey       ( IParameterPtr parameter, TimeType t );
bool    RemoveScaleKey          ( IParameterPtr parameter, TimeType t );
bool    RemoveTranslationKey    ( IParameterPtr parameter, TimeType t );
bool    RemoveCenterMassKey     ( IParameterPtr parameter, TimeType t );



// *******************************
//
bool                                                RemoveParameterKey ( IParameterPtr parameter, TimeType t );

bool                                                BezierSetCurveType ( IParameterPtr parameter, CurveType type );
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


} //model
} //bv

#include "GenericParameterSetters.inl"
