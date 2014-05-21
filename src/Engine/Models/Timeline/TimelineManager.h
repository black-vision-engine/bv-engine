#pragma once

#include <hash_map>
#include <vector>
#include <string>

#include "Engine/Models/Interfaces/ITimeEvaluator.h"

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "System/BasicTypes.h"

#include "Engine/Models/Timeline/Static/OffsetTimeEvaluator.h"
#include "Engine/Models/Timeline/Static/ConstTimeEvaluator.h"

#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "Engine/Models/Timeline/Dynamic/TimelineEventLoop.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventNull.h"
#include "Engine/Models/Timeline/Dynamic/TimelineEventStop.h"


namespace bv { namespace model {

class SimpleIParamSet : public IParamSet
{
private:

    std::vector< IParameter * >     m_parameters;

public:

    virtual std::vector< IParameter * > &       GetParameters       () override;
    virtual IParameter *                        GetParameter        ( const std::string & name ) override;

    bool                                        AddParameter        ( IParameter * param );
    bool                                        RemoveParameter     ( IParameter * param );
    unsigned int                                RemoveParameters    ( const std::string & name );

};


class TimelineManager
{
private:

    std::hash_map< std::string, ITimeEvaluatorPtr >         m_timelinesMap;
    std::hash_map< ITimeEvaluator *, SimpleIParamSet * >    m_registeredParams;

    ITimeEvaluatorPtr                                       m_rootTimeline;

public:

                            TimelineManager             ();
                            ~TimelineManager            ();

    ITimeEvaluatorPtr       CreateOffsetTimeEvaluatorStartingAt ( const std::string & name, TimeType startTime );
    ITimeEvaluatorPtr       CreateConstTimeEvaluator            ( const std::string & name, TimeType timeVal );
    DefaultTimelinePtr      CreateDefaultTimeline               ( const std::string & name, TimeType duration, TimelineWrapMethod preMethod, TimelineWrapMethod postMethod );
    
    bool                    AddStopEventToTimeline              ( ITimelinePtr timeline, const std::string & eventName, TimeType stopTime );

//    void                    RegisterRootTimeline    ( ITimeEvaluatorPtr root );
//
//    ITimeEvaluatorPtr       GetRootTimeline         ();
//
//    ITimeEvaluatorPtr       GetTimeline             ( const std::string & name );
//    IParamSet *             GetRegisteredParameters ( ITimeEvaluatorPtr timeline );
//    IParamSet *             GetRegisteredParameters ( const std::string & name );
//
//    bool                    RegisterOffsetTimeline  ( const std::string & name, TimeType offsetTime, std::string & parentName );
//    bool                    RegisterConstTimeline   ( const std::string & name, TimeType constTime, std::string & parentName );
//
//    bool                    RegisterTimeline        ( ITimeEvaluatorPtr timeline );
//
//    bool                    AddParamToTimeline      ( IParameter * param, const std::string & timelineName );
//    bool                    AddParamToTimeline      ( IParameter * param, ITimeEvaluatorPtr timeline );
//
//    unsigned int            RemoveFromTimeline      ( const std::string & paramName, const std::string & timelineName );
//    bool                    RemoveFromTimeline      ( IParameter * param, const std::string & timelineName );
//
//private:
//
//    SimpleIParamSet *       GetSimpleIParamSet      ( const std::string & timelineName );
//    bool                    AddParamToTimelineImpl  ( IParameter * param, ITimeEvaluatorPtr timeline );
//
//    bool                    RegisterOffsetTimeline  ( const std::string & name, TimeType offsetTime, ITimeEvaluatorPtr parent );
//    bool                    RegisterConstTimeline   ( const std::string & name, TimeType constTime, ITimeEvaluatorPtr parent );

};

} //model
} //bv

//// *********************************
////
//bool                                DefaultTimeline::AddParameter       ( IParameter * param )
//{
//    if( std::find( m_registeredParameters.begin(), m_registeredParameters.end(), param ) == m_registeredParameters.end() )
//    {
//        m_registeredParameters.push_back( param );
//        
//        return true;
//    }
//
//    return false;
//}
//
//// *********************************
////
//bool                                DefaultTimeline::RemoveParameter    ( const IParameter * param )
//{
//    auto it = std::find( m_registeredParameters.begin(), m_registeredParameters.end(), param );
//
//    if( it != m_registeredParameters.end() )
//    {
//        m_registeredParameters.erase( it );
//
//        return true;
//    }
//
//    return false;    
//}
//
//// *********************************
////
//unsigned int                        DefaultTimeline::RemoveParameters   ( const std::string & name )
//{
//    unsigned int erasedElements = 0;
//
//    for( auto it = m_registeredParameters.begin(); it != m_registeredParameters.end(); )
//    {
//        if( name == (*it)->GetName() )
//        {
//            it = m_registeredParameters.erase( it );
//
//            ++erasedElements;
//        }
//        else
//        {
//            ++it;
//        }
//    }
//
//    return erasedElements;
//}
