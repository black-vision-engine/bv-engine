#include "ConstTimeEvaluator.h"

namespace bv { namespace model {

// *******************************
//
ConstTimeEvaluator::ConstTimeEvaluator                      ( const std::string & name, TimeType val )
    : m_name( name )
    , m_timeVal( val )
{
}

// *******************************
//
ConstTimeEvaluator::~ConstTimeEvaluator                     ()
{
}

// *******************************
//
void                ConstTimeEvaluator::SetConstTimeValue   ( TimeType t )
{
    m_timeVal = t;
}

// *******************************
//
const std::string & ConstTimeEvaluator::GetName             () const
{
    return m_name;
}

// *******************************
//
void                ConstTimeEvaluator::SetGlobalTime       ( TimeType t )
{
}

// *******************************
//
TimeType            ConstTimeEvaluator::GetLocalTime        () const
{
    return m_timeVal;
}

} //model
} //bv
