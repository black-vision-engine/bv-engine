#pragma once

#include <hash_map>
#include <vector>
#include <string>

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"
#include "System/BasicTypes.h"


namespace bv { namespace model {

class ITimeEvaluator;

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
public:

    typedef std::vector< const ITimeEvaluator * >               TEvaluatorVec;

private:

    std::hash_map< std::string, const ITimeEvaluator * >        m_timelinesMap;
    std::hash_map< const ITimeEvaluator *, SimpleIParamSet * >  m_registeredParams;

public:

                            TimelineManager         ();
                            ~TimelineManager        ();


    TEvaluatorVec           GetTimelines            () const;

    const ITimeEvaluator *  GetTimeline             ( const std::string & name ) const;
    IParamSet *             GetRegisteredParameters ( const ITimeEvaluator * timeline );
    IParamSet *             GetRegisteredParameters ( const std::string & name );

    bool                    RegisterDefaultTimeline ( ITimeEvaluator * parent, TimeType startTime, TimeType endTime, const std::string & name );
    bool                    RegisterDefaultTimeline ( TimeType startTime, TimeType endTime, const std::string & name );
    bool                    RegisterTimeline        ( const ITimeEvaluator * timeline );

    bool                    AddParamToTimeline      ( IParameter * param, const std::string & timelineName );
    bool                    AddParamToTimeline      ( IParameter * param, const ITimeEvaluator * timeline );

    unsigned int            RemoveFromTimeline      ( const std::string & paramName, const std::string & timelineName );
    bool                    RemoveFromTimeline      ( IParameter * param, const std::string & timelineName );

private:

    SimpleIParamSet *       GetSimpleIParamSet      ( const std::string & timelineName );
    bool                    AddParamToTimelineImpl  ( IParameter * param, const ITimeEvaluator * timeline );

};

} //model
} //bv
