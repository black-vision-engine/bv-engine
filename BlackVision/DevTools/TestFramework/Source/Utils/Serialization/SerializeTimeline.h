#pragma once

#include "Engine/Models/Timeline/TimeEvaluatorBase.h"

#include "Serialize.h"

namespace bv {

model::ITimeEvaluatorPtr DeserializeTimeline( const std::string & path )
{
    BVXMLDeserializer deser( path, BVDeserializeContext::CreateContextFromEmptiness() );

    deser.EnterChild( "timeline" );
    auto object = model::TimeEvaluatorBase< model::ITimeEvaluator >::Create( deser );
    deser.ExitChild();

    return object;
}

}
