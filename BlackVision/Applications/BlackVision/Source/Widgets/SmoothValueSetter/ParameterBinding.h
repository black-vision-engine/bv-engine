#pragma once

#include "Engine/Models/Plugins/Parameters/GenericParameterSetters.h"
#include "Engine/Types/Values/BaseValue.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleTypedStates.h"


namespace bv { namespace nodelogic {


struct ParameterBinding
{
    enum class VectorComponent
    {
        X = 0,
        Y = 1,
        Z = 2,
        W = 3,

        Invalid
    };


    std::string                     Node;
    std::string                     Plugin;

    model::IParameterPtr            Parameter;
    IValuePtr                       ValueSrc;
    model::IStatedValuePtr          ValueState;

    TransformKind                   TransformKind;
    VectorComponent                 Component;

    ParameterBinding()
    {
        Parameter = nullptr;
        ValueSrc = nullptr;
        TransformKind = TransformKind::invalid;
        Component = VectorComponent::Invalid;
    }

};

}   // nodelogic
}	// bv
