#include "TimelineManager.h"

#include "Engine/Models/Interfaces/ITimeEvaluator.h"


namespace bv { namespace model {

// *********************************
//
TimelineManager::TimelineManager         ()
{
}

// *********************************
//
TimelineManager::~TimelineManager        ()
{
    //TODO: implement
}

// *********************************
//
const ITimeEvaluator *  TimelineManager::GetTimeline             ( const std::string & name ) const
{
    //TODO: implement
    return nullptr;
}

// *********************************
//
IParamSet *             TimelineManager::GetRegisteredParameters ( const ITimeEvaluator * timeline )
{
    //TODO: implement
    return nullptr;
}

// *********************************
//
IParamSet *             TimelineManager::GetRegisteredParameters ( const std::string & name )
{
    //TODO: implement
    return nullptr;
}

// *********************************
//
bool                    TimelineManager::RegisterDefaultTimeline ( const std::string & name )
{
    //TODO: implement
    return false;
}

// *********************************
//
bool                    TimelineManager::RegisterTimeline        ( const ITimeEvaluator * timeline )
{
    //TODO: implement
    return false;
}

// *********************************
//
bool                    TimelineManager::AddParamToTimeline      ( IParameter * param, const std::string & timelineName )
{
    //TODO: implement
    return false;
}

// *********************************
//
bool                    TimelineManager::AddParamToTimeline      ( IParameter * param, const ITimeEvaluator * timeline )
{
    //TODO: implement
    return false;
}

// *********************************
//
bool                    TimelineManager::RemoveFromTimeline      ( const std::string & paramName, const std::string & timelineName )
{
    //TODO: implement
    return false;
}

// *********************************
//
bool                    TimelineManager::RemoveFromTimeline      ( IParameter * param, const std::string & timelineName )
{
    //TODO: implement
    return false;
}

} //model
} //bv
