#include "stdafx.h"

#include "TimeEvaluatorBase.h"

#include "Dynamic/DefaultTimeline.h"
#include "Static/ConstTimeEvaluator.h"
#include "Static/OffsetTimeEvaluator.h"

#include "Serialization/IDeserializer.h"

#include "Serialization/BV/CloneViaSerialization.h"
#include "Assets/AssetDescsWithUIDs.h"



#include "Memory/MemoryLeaks.h"



namespace bv { 
    
// serialization stuff
//template std::vector< std::shared_ptr< model::TimeEvaluatorBase< model::ITimeEvaluator > > >                         DeserializeArray( const IDeserializer&, std::string name );

namespace model {

// *******************************
//
ITimeEvaluatorPtr            TimeEvaluatorBase< ITimeEvaluator >::Create              ( const IDeserializer& dob )
{
    auto type = dob.GetAttribute( "type" );

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

// *******************************
//
ITimeEvaluatorPtr           TimeEvaluatorBase< ITimeEvaluator >::CloneTyped    () const
{
    return CloneViaSerialization::Clone( this, "timeline", nullptr, nullptr );
}

// *******************************
//
template<>
ICloneablePtr               TimeEvaluatorBase< ITimeEvaluator >::Clone          () const
{
    return CloneTyped();
}

// *******************************
//
ITimeEvaluatorPtr           TimeEvaluatorBase< ITimeline >::CloneTyped          () const
{
    auto thisTE = reinterpret_cast< const TimeEvaluatorBase< ITimeEvaluator >* >( this );
    auto clone = CloneViaSerialization::Clone< TimeEvaluatorBase< ITimeEvaluator > >( thisTE, "timeline", nullptr, nullptr );
    return clone;
}

// *******************************
//
template<>
ICloneablePtr               TimeEvaluatorBase< ITimeline >::Clone               () const
{
    return CloneTyped();
}

} }
