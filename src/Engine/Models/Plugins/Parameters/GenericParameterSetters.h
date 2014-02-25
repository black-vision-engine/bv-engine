#pragma once

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


namespace bv { namespace model {

// *******************************
//
template< typename ValueType >
inline bool SetParameter( IParameter * parameter, TimeType t, const ValueType & val )
{
    return false;
}

bool    SetParameterRotation    ( IParameter * parameter, TimeType t, const glm::vec3 & rotAxis, float angle );
bool    SetParameterScale       ( IParameter * parameter, TimeType t, const glm::vec3 & scale );
bool    SetParameterTranslation ( IParameter * parameter, TimeType t, const glm::vec3 & translation );

bool    SetParameterRotation    ( IParameter * parameter, unsigned int idx, TimeType t, const glm::vec3 & rotAxis, float angle );
bool    SetParameterScale       ( IParameter * parameter, unsigned int idx, TimeType t, const glm::vec3 & scale );
bool    SetParameterTranslation ( IParameter * parameter, unsigned int idx, TimeType t, const glm::vec3 & translation );

} //model
} //bv

#include "GenericParameterSetters.inl"
