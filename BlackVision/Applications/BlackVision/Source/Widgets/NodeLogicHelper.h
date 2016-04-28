#pragma once

#include "Engine/Interfaces/IValue.h"

#include "Engine/Models/Timeline/TimelineHelper.h"

namespace bv
{

class ISerializer;
class IDeserializer;

namespace SerializationHelper
{


IValuePtr           CreateValue                         ( const IDeserializer & deser, const std::string & name );
void                SerializeValue                      ( ISerializer & ser, IValueConstPtr val );

} // SerializationHelper

model::ITimeEvaluatorPtr            GetDefaultTimeline      ( const IDeserializer & deser );

} // bv


