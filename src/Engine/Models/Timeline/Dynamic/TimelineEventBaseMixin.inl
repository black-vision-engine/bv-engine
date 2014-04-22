namespace bv { namespace model {

// *********************************
//
template< typename ITimelineEventIface >
inline TimelineEventBaseMixin< ITimelineEventIface >::TimelineEventBaseMixin                ( const std::string & name, TimelineEventType type, TimeType eventTime, const ITimeline * owner )
    : m_name( name )
    , m_type( type )
{
    SetEventTime( eventTime );
    SetOwnerTimeline( owner );
}

// *********************************
//
template< typename ITimelineEventIface >
inline std::string          TimelineEventBaseMixin< ITimelineEventIface >::GetName          () const
{
    return m_name;
}

// *********************************
//
template< typename ITimelineEventIface >
inline TimelineEventType    TimelineEventBaseMixin< ITimelineEventIface >::GetType          () const
{
    return m_type;
}

// *********************************
//
template< typename ITimelineEventIface >
inline TimeType         TimelineEventBaseMixin< ITimelineEventIface >::GetEventTime         () const
{
    return m_eventTime;
}
    
// *********************************
//
template< typename ITimelineEventIface >
const ITimeline *       TimelineEventBaseMixin< ITimelineEventIface >::GetOwnerTimeline     () const
{
    return m_owner;
}

// *********************************
//
template< typename ITimelineEventIface >
inline void                 TimelineEventBaseMixin< ITimelineEventIface >::SetOwnerTimeline ( const ITimeline * owner )
{
    m_owner = owner;
}

// *********************************
//
template< typename ITimelineEventIface >
inline void                 TimelineEventBaseMixin< ITimelineEventIface >::SetEventTime     ( TimeType eventTime )
{
    m_eventTime = eventTime;
}

} //model
} //bv
