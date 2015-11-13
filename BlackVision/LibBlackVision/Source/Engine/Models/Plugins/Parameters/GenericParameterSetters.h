#pragma once

#include "Engine/Models/Plugins/Parameters/CachedSimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"

#include "Mathematics/Interpolators/CompositeBezierInterpolator.h"

namespace bv { namespace model {

// *******************************
//
template< typename ValueType >
inline bool SetParameter( IParameterPtr parameter, TimeType t, const ValueType & val )
{
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


// *******************************
//

bool                                                BezierSetCurveType( IParameterPtr parameter, CurveType type );
CurveType                                           BezierGetCurveType( IParameterPtr parameter );

void                                                BezierSetWrapPostMethod  ( IParameterPtr parameter, WrapMethod method );
void                                                BezierSetWrapPreMethod   ( IParameterPtr parameter, WrapMethod method );
WrapMethod                                          BezierGetWrapPostMethod  ( IParameterPtr parameter );
WrapMethod                                          BezierGetWrapPreMethod   ( IParameterPtr parameter );

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
