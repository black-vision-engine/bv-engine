#pragma once

#include <cassert>
#include <string>

#include "Engine/Interfaces/ITimeEvaluator.h"
#include "Engine/Types/Enums.h"

#include "System/BasicTypes.h"


namespace bv { namespace model {

class ITimeEvaluator;

//FIXME: this shit deserves some templates :D
class BaseParameter
{
protected:

    const ITimeEvaluator *  m_timeEvaluator;

    std::string             m_name;
    mutable TimeType        m_lastEvaluatedTime;

protected:

    explicit    BaseParameter  ( const std::string & name, const ITimeEvaluator * evaluator = nullptr );
    virtual     ~BaseParameter ();

    inline void                 SetLastEvaluatedTime    ( TimeType t )  const;
    inline TimeType             GetLastEvaluatedTime    ()              const;

public:

    virtual std::string         GetName                 ()  const;

    void                        SetTimeEvaluator        ( const ITimeEvaluator * timeEvaluator );
    inline bool                 IsEvaluationNeeded      ( TimeType t )  const;

protected:

    inline TimeType             GetEvaluationTime       ( TimeType t ) const;

};

} //model
} //bv

#include "BaseParameter.inl"