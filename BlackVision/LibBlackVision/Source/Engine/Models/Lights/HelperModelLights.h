#pragma once

#include "Engine/Types/Enums.h"
#include "Engine/Models/Interfaces/IModelLight.h"
#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {


class HelperModelLights
{
public:

    static IModelLight *                                    CreateModelLight        ( LightType type, ITimeEvaluatorPtr timeEvaluator );

};

} //model
} //bv
