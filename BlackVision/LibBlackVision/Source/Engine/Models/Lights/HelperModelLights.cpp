#include "stdafx.h"

#include "HelperModelLights.h"
#include "Engine/Models/Lights/ModelDirectionalLight.h"
#include "Engine/Models/Lights/ModelPointLight.h"
#include "Engine/Models/Lights/ModelSpotLight.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"




#include "Memory/MemoryLeaks.h"



namespace bv {
namespace model
{


// *************************************
//
ModelBaseLight *                HelperModelLights::CreateModelLight              ( LightType type, ITimeEvaluatorPtr timeEvaluator )
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


namespace Convert
{

// ***********************
//
std::pair< LightType, const char* > lt2s[] = 
{
    std::make_pair( LightType::LT_DIRECTIONAL, "directional" )
    , std::make_pair( LightType::LT_POINT, "point" )
    , std::make_pair( LightType::LT_SPOT, "spot" )
    , std::make_pair( LightType::LT_TOTAL, "" )
};

IMPLEMENT_ENUM_SERIALIZATION( LightType, lt2s )


} // Convert

} //bv
