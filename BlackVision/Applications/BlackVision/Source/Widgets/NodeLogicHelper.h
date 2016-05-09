#pragma once

#include "Engine/Interfaces/IValue.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultParamValModel.h"
#include "Engine/Models/Plugins/ParamValModel/ParamValEvaluatorFactory.h"

#include "Engine/Models/Timeline/TimelineHelper.h"

namespace bv
{

class ISerializer;
class IDeserializer;

namespace SerializationHelper
{


IValuePtr           CreateValue                         ( const IDeserializer & deser, const std::string & name );
void                SerializeValue                      ( ISerializer & ser, IValueConstPtr val );

model::ITimeEvaluatorPtr            GetDefaultTimeline  ( const IDeserializer & deser );

} // SerializationHelper


model::SimpleFloatEvaluatorPtr      AddFloatParam       ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const float & initVal );
model::SimpleBoolEvaluatorPtr       AddBoolParam        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const bool & initVal );
model::SimpleVec2EvaluatorPtr       AddVec2Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec2 & initVal );
model::SimpleVec3EvaluatorPtr       AddVec3Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec3 & initVal );
model::SimpleVec4EvaluatorPtr       AddVec4Param        ( model::DefaultParamValModelPtr paramModel, model::ITimeEvaluatorPtr timeEvaluator, const std::string & paramName, const glm::vec4 & initVal );

} // bv


