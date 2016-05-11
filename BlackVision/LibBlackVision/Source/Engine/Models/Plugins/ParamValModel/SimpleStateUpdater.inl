
namespace bv { namespace model {

// ********************************
//
template< typename StateTypePtr, typename ValueTypePtr >
SimpleStateUpdater<StateTypePtr,ValueTypePtr>::SimpleStateUpdater( StateTypePtr state, ValueTypePtr value )
    : m_state( state )
    , m_value( value )  
{
}

// ********************************
//
template< typename StateTypePtr, typename ValueTypePtr >
void    SimpleStateUpdater<StateTypePtr,ValueTypePtr>::DoUpdate    () 
{
    m_state->Update( m_value->GetValue() );
}

// ***********************
//
template< typename StateTypePtr, typename ValueTypePtr >
const std::string &     SimpleStateUpdater<StateTypePtr,ValueTypePtr>::GetName     ()
{
    return m_value->GetName();
}

} // model
} // bvc
