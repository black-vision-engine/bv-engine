#include "TimeEvaluatorBase.h"

#include "Dynamic/DefaultTimeline.h"
#include "Static/ConstTimeEvaluator.h"
#include "Static/OffsetTimeEvaluator.h"

#include "Engine/Serialization/SerializationObjects.inl"

namespace bv { 
    
// serialization stuff
template std::vector< std::shared_ptr< model::TimeEvaluatorBase< model::ITimeEvaluator > > >                         DeserializeObjectLoadArrayImpl( DeserializeObjectImpl*, std::string name );

namespace model {

// *******************************
//
ISerializablePtr     TimeEvaluatorBase< ITimeEvaluator >::Create              ( DeserializeObject & dob )
{
    auto type = dob.GetValue( "type" );

    if( type == "default" )
        return DefaultTimeline::Create( dob );
    else if( type == "offset" )
        return OffsetTimeEvaluator::Create( dob );
    else if( type == "const" )
        return ConstTimeEvaluator::Create( dob );
    else
    {
        assert( false );
        return nullptr;
    }
}

} }
