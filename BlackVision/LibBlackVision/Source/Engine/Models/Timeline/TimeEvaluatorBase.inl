#include "TimeEvaluatorBase.h"
namespace bv { namespace model {

// *******************************
//
template< typename ITimeEvaluatorIface >
TimeEvaluatorBase< ITimeEvaluatorIface >::TimeEvaluatorBase   ( const std::string & name )
    : m_name( name )
{
}

// *******************************
//
template< typename ITimeEvaluatorIface >
TimeEvaluatorBase< ITimeEvaluatorIface >::~TimeEvaluatorBase  ()
{
}

// *******************************
//
template< typename ITimeEvaluatorIface >
void                                        TimeEvaluatorBase< ITimeEvaluatorIface >::AddChild      ( ITimeEvaluatorPtr child )
{
    //FIXME: assert no duplicates (if necessary)
    assert( child != nullptr );
    m_children.push_back( child );
}

// *******************************
//
template< typename ITimeEvaluatorIface >
ITimeEvaluatorPtr                           TimeEvaluatorBase< ITimeEvaluatorIface >::GetChild      ( const std::string & name )
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
template< typename ITimeEvaluatorIface >
const std::vector< ITimeEvaluatorPtr > &    TimeEvaluatorBase< ITimeEvaluatorIface >::GetChildren   ()
{
    return m_children;
}

// *******************************
//
template< typename ITimeEvaluatorIface >
bool                                        TimeEvaluatorBase< ITimeEvaluatorIface >::RemoveChild   ( ITimeEvaluatorPtr child )
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
template< typename ITimeEvaluatorIface >
bool                                        TimeEvaluatorBase< ITimeEvaluatorIface >::RemoveChild   ( const std::string & name )
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
template< typename ITimeEvaluatorIface >
void										TimeEvaluatorBase< ITimeEvaluatorIface >::SetName       ( std::string name )
{
    m_name = name;
}

// *******************************
//
template< typename ITimeEvaluatorIface >
const std::string &                         TimeEvaluatorBase< ITimeEvaluatorIface >::GetName       () const
{
    return m_name;
}

// *******************************
//
template< typename ITimeEvaluatorIface >
void                                        TimeEvaluatorBase< ITimeEvaluatorIface >::SetGlobalTime ( TimeType t )
{
    SetGlobalTimeImpl( t );
    t = GetLocalTime();

    //for( auto c : m_children )
    for( unsigned int i = 0; i < m_children.size(); ++i )
    {
        auto & child = m_children[ i ];
        child->SetGlobalTime( t );
    }
}

// ***********************
//
template<typename ITimeEvaluatorIface>
inline void TimeEvaluatorBase<ITimeEvaluatorIface>::SerializeChildren( ISerializer & ser ) const
{
    ser.EnterArray( "children" );
    for( auto child : m_children )
        child->Serialize( ser );
    ser.ExitChild(); // children
}

template<typename ITimeEvaluatorIface>
inline void TimeEvaluatorBase<ITimeEvaluatorIface>::DeserializeChildren( const IDeserializer & deser )
{
    auto children = SerializationHelper::DeserializeArray< TimeEvaluatorBase< ITimeEvaluator > >( deser, "children", "timeline" );

    for( auto child : children )
        AddChild( child );
}

} //model
} //bv
