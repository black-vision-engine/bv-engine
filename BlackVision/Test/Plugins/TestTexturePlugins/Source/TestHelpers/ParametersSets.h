#pragma once


#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"


namespace bv
{


// ***********************
//
class ParametersSets
{
public:

    static void         SetSamplerStateParamsTestSet1       ( model::IParamValModelPtr resStateModel );
    static bool         CompareToSamplerStateParamsTestSet1 ( model::IParamValModelPtr resStateModel );
};






}	// bv

