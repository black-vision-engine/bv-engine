namespace bv { namespace model {

// *********************************
//
template< typename ITimelineEventIface >
inline TimelineEventBaseMixin< ITimelineEventIface >::TimelineEventBaseMixin                    ( const std::string & name, TimelineEventType type, TimeType eventTime, const ITimeline * owner )
    : m_name( name )
    , m_type( type )
    , m_active( true )
{
    SetEventTime( eventTime );
    //SetLastTriggerTime( -1.f ); //FIXME: magic number - negative time seems to be good enough for this purpose though (as negative global time id not allowed)
    SetOwnerTimeline( owner );
}

// *********************************
//
template< typename ITimelineEventIface >
inline std::string          TimelineEventBaseMixin< ITimelineEventIface >::GetName              () const
{
    return m_name;
}

// *********************************
//
template< typename ITimelineEventIface >
inline TimelineEventType    TimelineEventBaseMixin< ITimelineEventIface >::GetType              () const
{
    return m_type;
}

// *********************************
//
template< typename ITimelineEventIface >
inline bool                 TimelineEventBaseMixin< ITimelineEventIface >::IsActive             () const
{
    return m_active;
}

// *********************************
//
template< typename ITimelineEventIface >
inline void             TimelineEventBaseMixin< ITimelineEventIface >::SetActive                ( bool active )
{
    m_active = active;
}

// *********************************
//
template< typename ITimelineEventIface >
inline TimeType         TimelineEventBaseMixin< ITimelineEventIface >::GetEventTime             () const
{
    return m_eventTime;
}

//// *********************************
////
//template< typename ITimelineEventIface >
//inline TimeType         TimelineEventBaseMixin< ITimelineEventIface >::GetLastTriggerTime       () const
//{
//    return m_lastTriggerTime;
//}
//
//// *********************************
////
//template< typename ITimelineEventIface >
//inline void                 TimelineEventBaseMixin< ITimelineEventIface >::SetLastTriggerTime   ( TimeType triggerTime )
//{
//    m_lastTriggerTime = triggerTime;
//}

// *********************************
//
template< typename ITimelineEventIface >
const ITimeline *       TimelineEventBaseMixin< ITimelineEventIface >::GetOwnerTimeline         () const
{
    return m_owner;
}

// *********************************
//
template< typename ITimelineEventIface >
inline void                 TimelineEventBaseMixin< ITimelineEventIface >::SetOwnerTimeline     ( const ITimeline * owner )
{
    m_owner = owner;
}

// *********************************
//
template< typename ITimelineEventIface >
inline void                 TimelineEventBaseMixin< ITimelineEventIface >::SetEventTime         ( TimeType eventTime )
{
    m_eventTime = eventTime;
}

} //model
} //bv
