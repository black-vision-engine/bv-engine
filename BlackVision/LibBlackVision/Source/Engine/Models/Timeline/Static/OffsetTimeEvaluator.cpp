#include "OffsetTimeEvaluator.h"

namespace bv { namespace model {

// *******************************
//
OffsetTimeEvaluator::OffsetTimeEvaluator                    ( const std::string & name, TimeType offsetTime )
    : Parent( name )
    , m_globalTime( 0.f )
    , m_timeOffset( offsetTime )
{
}

// *******************************
//
OffsetTimeEvaluator::~OffsetTimeEvaluator                   ()
{
}

// *******************************
//
void                OffsetTimeEvaluator::Serialize           ( SerializeObject & sob ) const
{
    sob.SetName( "timeline" );
    sob.SetValue( "name", GetName() );
    sob.SetValue( "type", "offset" );

    sob.SetName( "children" );
    for( auto child : m_children )
        child->Serialize( sob );
    sob.Pop(); // children

    sob.Pop();
}

// *******************************
//
ISerializablePtr     OffsetTimeEvaluator::Create              ( DeserializeObject & dob )
{
    auto name = dob.GetValue( "name" );

    auto te = std::make_shared< OffsetTimeEvaluator >( name, 0.f ); // FIXME load offset

    auto children = dob.LoadArray< TimeEvaluatorBase< ITimeEvaluator > >( "children" );

    for( auto child : children )
        te->AddChild( child );

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
    return m_globalTime + m_timeOffset;
}

} //model
} //bv
