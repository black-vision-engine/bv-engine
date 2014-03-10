#pragma once

#include <hash_map>
#include <vector>
#include <string>

#include "Engine/Models/Plugins/Interfaces/IParamSet.h"


namespace bv { namespace model {

class ITimeEvaluator;

class SimpleIParamSet : public IParamSet
{
public:

    virtual std::vector< IParameter * > &       GetParameters       () override;
    virtual IParameter *                        GetParameter        ( const std::string & name ) override;

    bool                                        AddParameter        ( IParameter * param );
    bool                                        RemoveParameter     ( IParameter * param );
    bool                                        RemoveParameter     ( const std::string & name );

};


class TimelineManager
{
private:

    std::vector< const ITimeEvaluator * >                       m_timelinesVec;
    std::hash_map< std::string, const ITimeEvaluator * >        m_timelinesMap;

    std::hash_map< const ITimeEvaluator *, SimpleIParamSet * >  m_registeredParams;

public:

                            TimelineManager         ();
                            ~TimelineManager        ();


    const ITimeEvaluator *  GetTimeline             ( const std::string & name ) const;
    IParamSet *             GetRegisteredParameters ( const ITimeEvaluator * timeline );
    IParamSet *             GetRegisteredParameters ( const std::string & name );

    bool                    RegisterDefaultTimeline ( const std::string & name );
    bool                    RegisterTimeline        ( const ITimeEvaluator * timeline );

    bool                    AddParamToTimeline      ( IParameter * param, const std::string & timelineName );
    bool                    AddParamToTimeline      ( IParameter * param, const ITimeEvaluator * timeline );

    bool                    RemoveFromTimeline      ( const std::string & paramName, const std::string & timelineName );
    bool                    RemoveFromTimeline      ( IParameter * param, const std::string & timelineName );

};

} //model
} //bv
