#include "stdafx.h"

#include "OffsetTimeEvaluator.h"
#include "Serialization/ISerializer.h"

#include "Serialization/SerializationHelper.h"
#include "Serialization/SerializationHelper.inl"



#include "Memory/MemoryLeaks.h"



namespace bv { namespace model {

// *******************************
//
OffsetTimeEvaluator::OffsetTimeEvaluator                    ( const std::string & name, TimeType offsetTime, TimeType scale )
    : Parent( name )
    , m_globalTime( 0.f )
    , m_timeOffset( offsetTime )
    , m_timeScale( scale )
{
}

// *******************************
//
OffsetTimeEvaluator::~OffsetTimeEvaluator                   ()
{
}

// *******************************
//
void                OffsetTimeEvaluator::Serialize           ( ISerializer& ser ) const
{
    ser.EnterChild( "timeline" );
    ser.SetAttribute( "name", GetName() );
    ser.SetAttribute( "type", "offset" );
    SerializationHelper::SerializeAttribute( ser, m_timeOffset, "offset" );
    SerializationHelper::SerializeAttribute( ser, m_timeScale, "scale" );

    SerializeChildren( ser ); // this is needed since this is scene timeline and it has children

    ser.ExitChild();
}

// *******************************
//
OffsetTimeEvaluatorPtr     OffsetTimeEvaluator::Create             ( const std::string & name, TimeType offsetTime, TimeType scale )
{
    return OffsetTimeEvaluatorPtr( new OffsetTimeEvaluator( name, offsetTime, scale ) );
}

// *******************************
//
OffsetTimeEvaluatorPtr     OffsetTimeEvaluator::Create              ( const IDeserializer& deser )
{
    auto name = deser.GetAttribute( "name" );

    auto offset = Convert::String2T< float >( deser.GetAttribute( "offset" ), 0.f );
    auto scale = Convert::String2T< float >( deser.GetAttribute( "scale" ), 1.f );

    auto te = OffsetTimeEvaluator::Create( name, offset, scale );

//    te->DeserializeChildren( deser );

    return te;
}

// *******************************
//
void                OffsetTimeEvaluator::SetTimeOffset      ( TimeType t )
{
    m_timeOffset = t;
}

// *******************************
//
void                OffsetTimeEvaluator::SetGlobalTimeImpl  ( TimeType t )
{
    m_globalTime = t;
}

// *******************************
//
TimeType            OffsetTimeEvaluator::GetLocalTime       () const
{
    return ( m_globalTime + m_timeOffset ) * m_timeScale;
}

// ***********************
//
const std::string&        OffsetTimeEvaluator::GetType             () const
{
    return Type();
}

// ***********************
//
namespace {
const std::string OffsetTimeEvaluatorType = "OffsetTimeEvaluator";
} //annonymous

// ***********************
//
const std::string&        OffsetTimeEvaluator::Type                ()
{
    return OffsetTimeEvaluatorType;
}

} //model
} //bv
