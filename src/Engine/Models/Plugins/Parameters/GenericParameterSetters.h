#pragma once

#include "Engine/Models/Plugins/Parameters/SimpleTypedParameters.h"
#include "Engine/Models/Plugins/Parameters/CompositeTypedParameters.h"


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

bool    SetParameterRotation    ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & rotAxis, float angle );
bool    SetParameterScale       ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & scale );
bool    SetParameterTranslation ( IParameterPtr parameter, unsigned int idx, TimeType t, const glm::vec3 & translation );

} //model
} //bv

#include "GenericParameterSetters.inl"
