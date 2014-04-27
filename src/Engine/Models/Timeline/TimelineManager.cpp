#include "TimelineManager.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IParameter.h"
#include "Engine/Models/Timeline/Static/Timeline.h"


namespace bv { namespace model {

// ******************************************************** SimpleIParamSet ********************************************************

// *********************************
//
std::vector< IParameter * > &       SimpleIParamSet::GetParameters       ()
{
    return m_parameters;
}

// *********************************
//
IParameter *                        SimpleIParamSet::GetParameter        ( const std::string & name )
{
    for( auto param : m_parameters )
    {
        if( param->GetName() == name )
        {
            return param;
        }
    }

    return nullptr;
}

// *********************************
//Name duplicates are allowed, but stored pointers must me be unique
bool                             SimpleIParamSet::AddParameter        ( IParameter * param )
{
    if( std::find( m_parameters.begin(), m_parameters.end(), param ) == m_parameters.end() )
    {
        m_parameters.push_back( param );
        
        return true;
    }

    return false;
}

// *********************************
//
bool                                SimpleIParamSet::RemoveParameter     ( IParameter * param )
{
    auto it = std::find( m_parameters.begin(), m_parameters.end(), param );

    if( it != m_parameters.end() )
    {
        m_parameters.erase( it );

        return true;
    }

    return false;    
}

// *********************************
//
unsigned int                        SimpleIParamSet::RemoveParameters    ( const std::string & name )
{
    unsigned int erasedElements = 0;

    for( auto elt = m_parameters.begin(); elt != m_parameters.end(); )
    {
        if( name == (*elt)->GetName() )
        {
            elt = m_parameters.erase( elt );

            ++erasedElements;
        }
        else
        {
            ++elt;
        }
    }

    return erasedElements;
}

// ******************************************************** TimelineManager ********************************************************

// *********************************
//
TimelineManager::TimelineManager         ()
{
}

// *********************************
//
TimelineManager::~TimelineManager        ()
{
    for( auto elt : m_registeredParams )
    {
        delete elt.first;
        delete elt.second;
    }
}

// *********************************
//
auto                    TimelineManager::GetTimelines            () const -> TEvaluatorVec
{
    TEvaluatorVec retVec;

    for( auto elt : m_timelinesMap )
    {
        retVec.push_back( elt.second );
    }

    return retVec;
}

// *********************************
//
const ITimeEvaluator *  TimelineManager::GetTimeline             ( const std::string & name ) const
{
    auto it = m_timelinesMap.find( name );

    if( it != m_timelinesMap.end() )
    {
        return it->second;
    }

    return nullptr;
}

// *********************************
//
IParamSet *             TimelineManager::GetRegisteredParameters ( const ITimeEvaluator * timeline )
{
    auto it = m_registeredParams.find( timeline );

    if( it != m_registeredParams.end() )
    {
        return it->second;
    }
    
    return nullptr;
}

// *********************************
//
IParamSet *             TimelineManager::GetRegisteredParameters ( const std::string & name )
{
    return GetRegisteredParameters( GetTimeline( name ) );
}

// *********************************
//
bool                    TimelineManager::RegisterDefaultTimeline ( ITimeEvaluator * parent, TimeType startTime, TimeType endTime, const std::string & name )
{
    if( GetTimeline( name ) == nullptr )
    {
        ITimeEvaluator * timeline = new Timeline( parent, startTime, endTime, name );

        return RegisterTimeline( timeline );
    }

    return false;
}

// *********************************
//
bool                    TimelineManager::RegisterDefaultTimeline ( TimeType startTime, TimeType endTime, const std::string & name )
{
    return RegisterDefaultTimeline( nullptr, startTime, endTime, name );
}

// *********************************
//
bool                    TimelineManager::RegisterTimeline        ( const ITimeEvaluator * timeline )
{
    if( GetTimeline( timeline->GetName() ) == nullptr )
    {
        assert( m_registeredParams.find( timeline ) == m_registeredParams.end() );
        
        m_timelinesMap[ timeline->GetName() ] = timeline;
        m_registeredParams[ timeline ] = new SimpleIParamSet();

        return true;
    }

    return false;
}

// *********************************
//
bool                    TimelineManager::AddParamToTimeline      ( IParameter * param, const std::string & timelineName )
{
    auto timeline = GetTimeline( timelineName );

    if( timeline != nullptr )
    {
        return AddParamToTimelineImpl( param, timeline );
    }

    return false;
}

// *********************************
//
bool                    TimelineManager::AddParamToTimeline      ( IParameter * param, const ITimeEvaluator * timeline )
{
    auto tl = GetTimeline( timeline-> GetName() );

    assert( tl == nullptr || tl == timeline );

    if( tl != nullptr )
    {
        return AddParamToTimelineImpl( param, tl );
    }

    if ( !RegisterTimeline( timeline ) )
    {
        assert( false );

        return false;
    }

    return AddParamToTimeline( param, timeline );
}

// *********************************
//
unsigned int            TimelineManager::RemoveFromTimeline      ( const std::string & paramName, const std::string & timelineName )
{
    auto paramSet = GetSimpleIParamSet( timelineName );

    if( paramSet )
    {
        return paramSet->RemoveParameters( paramName );
    }

    return 0;
}

// *********************************
//
bool                    TimelineManager::RemoveFromTimeline      ( IParameter * param, const std::string & timelineName )
{
    auto paramSet = GetSimpleIParamSet( timelineName );

    if( paramSet )
    {
        paramSet->RemoveParameter( param );
    }

    return false;
}

// *********************************
//
SimpleIParamSet *       TimelineManager::GetSimpleIParamSet      ( const std::string & timelineName )
{
    auto tl = GetTimeline( timelineName );

    if( tl != nullptr )
    {
        assert( m_registeredParams.find( tl ) != m_registeredParams.end() );

        return m_registeredParams[ tl ];
    }

    return nullptr;
}

// *********************************
//
bool                    TimelineManager::AddParamToTimelineImpl ( IParameter * param, const ITimeEvaluator * timeline )
{
    auto paramSet = m_registeredParams[ timeline ];

    assert( paramSet != nullptr );

    bool bAdded = paramSet->AddParameter( param );
    
    if ( bAdded )
    {
        SetParamTimeline( param, timeline );
    }

    return bAdded;
}

} //model
} //bv
