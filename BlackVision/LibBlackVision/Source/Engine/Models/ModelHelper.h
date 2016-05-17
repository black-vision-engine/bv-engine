#pragma once

#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleTypedStates.h"
#include "Engine/Models/Plugins/ParamValModel/SimpleStateUpdater.h"

#include "Engine/Models/Timeline/TimelineHelper.h"


namespace bv { namespace model {


model::SimpleFloatEvaluatorPtr      AddFloatParam       ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const float & initVal, bool addState = false );
model::SimpleIntEvaluatorPtr        AddIntParam         ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const int & initVal, bool addState = false );
model::SimpleBoolEvaluatorPtr       AddBoolParam        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const bool & initVal, bool addState = false );
model::SimpleVec2EvaluatorPtr       AddVec2Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec2 & initVal, bool addState = false );
model::SimpleVec3EvaluatorPtr       AddVec3Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec3 & initVal, bool addState = false );
model::SimpleVec4EvaluatorPtr       AddVec4Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec4 & initVal, bool addState = false );


template< typename ValueType, ParamType ParamTypeEnum >
void                                AddState            ( model::DefaultParamValModelPtr & model, std::shared_ptr< ValueImpl< ValueType, ParamTypeEnum > > value, const std::string & valueName )
{
    auto state = std::make_shared< model::SimpleState< ValueType > >();
    auto updater = std::make_shared< 
        model::SimpleStateUpdater< 
            std::shared_ptr< model::SimpleState< ValueType > >, 
            std::shared_ptr< ValueImpl< ValueType, ParamTypeEnum > >
        > >( state, value );

    model->AddState( valueName, state, updater );
}

} // model

namespace SerializationHelper
{

model::ITimeEvaluatorPtr            GetDefaultTimeline  ( const IDeserializer & deser );


} //SerializationHelper

} //bv
