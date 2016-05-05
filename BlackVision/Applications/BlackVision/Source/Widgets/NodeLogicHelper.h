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

} // bv


