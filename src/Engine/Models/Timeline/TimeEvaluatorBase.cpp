#include "TimeEvaluatorBase.h"

#include <cassert>


namespace bv { namespace model {

// *******************************
//
TimeEvaluatorBase::TimeEvaluatorBase   ( const std::string & name )
    : m_name( name )
{
}

// *******************************
//
TimeEvaluatorBase::~TimeEvaluatorBase  ()
{
}

// *******************************
//
void                                        TimeEvaluatorBase::AddChild            ( ITimeEvaluatorPtr child )
{
    //FIXME: assert no duplicates (if necessary)
    assert( child != nullptr );
    m_children.push_back( child );
}

// *******************************
//
ITimeEvaluatorPtr                           TimeEvaluatorBase::GetChild            ( const std::string & name )
{
    for( auto c : m_children )
    {
        if( c->GetName() == name )
        {
            return c;
        }
    }

    return nullptr;
}

// *******************************
//
const std::vector< ITimeEvaluatorPtr > &    TimeEvaluatorBase::GetChildren         ()
{
    return m_children;
}

// *******************************
//
bool                                        TimeEvaluatorBase::RemoveChild         ( ITimeEvaluatorPtr child )
{
    for( auto it = m_children.begin(); it != m_children.end(); ++it )
    {
        if( *it == child )
        {
            m_children.erase( it );

            return true;
        }
    }

    return false;
}

// *******************************
//
bool                                        TimeEvaluatorBase::RemoveChild         ( const std::string & name )
{
    for( auto it = m_children.begin(); it != m_children.end(); ++it )
    {
        if( (*it)->GetName() == name )
        {
            m_children.erase( it );

            return true;
        }
    }

    return false;
}

// *******************************
//
const std::string &                         TimeEvaluatorBase::GetName             () const
{
    return m_name;
}

} //model
} //bv
