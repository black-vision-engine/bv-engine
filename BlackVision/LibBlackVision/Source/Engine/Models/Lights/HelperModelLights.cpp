#include "stdafx.h"

#include "HelperModelLights.h"
#include "Engine/Models/Lights/ModelDirectionalLight.h"
#include "Engine/Models/Lights/ModelPointLight.h"
#include "Engine/Models/Lights/ModelSpotLight.h"


namespace bv { namespace model {


// *************************************
//
IModelLight *                   HelperModelLights::CreateModelLight              ( LightType type, ITimeEvaluatorPtr timeEvaluator )
{
    if( type == LightType::LT_DIRECTIONAL )
    {
        return new ModelDirectionalLight( timeEvaluator );
    }
    else if( type == LightType::LT_POINT )
    {
        return new ModelPointLight( timeEvaluator );
    }
    else if( type == LightType::LT_SPOT )
    {
        return new ModelSpotLight( timeEvaluator );
    }
    else
    {
        assert( false ); // light type missing
    }

    return nullptr;
}

} //model
} //bv
