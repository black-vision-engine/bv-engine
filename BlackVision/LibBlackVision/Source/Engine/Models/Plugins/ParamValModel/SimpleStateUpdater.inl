namespace bv { namespace model {

template< typename StateTypePtr, typename ValueTypePtr >
SimpleStateUpdater<StateTypePtr,ValueTypePtr>::SimpleStateUpdater( StateTypePtr state, ValueTypePtr value )
    : m_state( state )
    , m_value( value )  
{
}

template< typename StateTypePtr, typename ValueTypePtr >
void    SimpleStateUpdater<StateTypePtr,ValueTypePtr>::DoUpdate    () 
{
    m_state->Update( m_value->GetValue() );
}

} }
