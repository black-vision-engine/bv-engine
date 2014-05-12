namespace bv { namespace model {

//// *******************************
////
//template< typename ITimeEvaluatorIface >
//TimeEvaluatorBase< ITimeEvaluatorIface >::TimeEvaluatorBase   ( const std::string & name )
//    : m_name( name )
//{
//}
//
//// *******************************
////
//template< typename ITimeEvaluatorIface >
//TimeEvaluatorBase< ITimeEvaluatorIface >::~TimeEvaluatorBase  ()
//{
//}
//
//// *******************************
////
//template< typename ITimeEvaluatorIface >
//void                                        TimeEvaluatorBase< ITimeEvaluatorIface >::AddChild      ( ITimeEvaluatorPtr child )
//{
//    //FIXME: assert no duplicates (if necessary)
//    assert( child != nullptr );
//    m_children.push_back( child );
//}
//
//// *******************************
////
//template< typename ITimeEvaluatorIface >
//ITimeEvaluatorPtr                           TimeEvaluatorBase< ITimeEvaluatorIface >::GetChild      ( const std::string & name )
//{
//    for( auto c : m_children )
//    {
//        if( c->GetName() == name )
//        {
//            return c;
//        }
//    }
//
//    return nullptr;
//}
//
//// *******************************
////
//template< typename ITimeEvaluatorIface >
//const std::vector< ITimeEvaluatorPtr > &    TimeEvaluatorBase< ITimeEvaluatorIface >::GetChildren   ()
//{
//    return m_children;
//}
//
//// *******************************
////
//template< typename ITimeEvaluatorIface >
//bool                                        TimeEvaluatorBase< ITimeEvaluatorIface >::RemoveChild   ( ITimeEvaluatorPtr child )
//{
//    for( auto it = m_children.begin(); it != m_children.end(); ++it )
//    {
//        if( *it == child )
//        {
//            m_children.erase( it );
//
//            return true;
//        }
//    }
//
//    return false;
//}
//
//// *******************************
////
//template< typename ITimeEvaluatorIface >
//bool                                        TimeEvaluatorBase< ITimeEvaluatorIface >::RemoveChild   ( const std::string & name )
//{
//    for( auto it = m_children.begin(); it != m_children.end(); ++it )
//    {
//        if( (*it)->GetName() == name )
//        {
//            m_children.erase( it );
//
//            return true;
//        }
//    }
//
//    return false;
//}
//
//// *******************************
////
//template< typename ITimeEvaluatorIface >
//const std::string &                         TimeEvaluatorBase< ITimeEvaluatorIface >::GetName       () const
//{
//    return m_name;
//}
//
//// *******************************
////
//template< typename ITimeEvaluatorIface >
//void                                        TimeEvaluatorBase< ITimeEvaluatorIface >::SetGlobalTime ( TimeType t )
//{
//    SetGlobalTimeImpl( t );
//    t = GetLocalTime();
//
//    for( auto c : m_children )
//    {
//        c->SetGlobalTime( t );
//    }
//}

} //model
} //bv
