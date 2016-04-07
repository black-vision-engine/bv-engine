#pragma once

#include "Engine/Types/Enums.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

class ModelBaseLight;

class HelperModelLights
{
public:

    static ModelBaseLight *         CreateModelLight        ( LightType type, ITimeEvaluatorPtr timeEvaluator );

};

} //model
} //bv
