#pragma once

#include <vector>

#include "System/BasicTypes.h"
#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"


namespace bv {

class AICommandBase;

class TestAI
{
private:

    std::vector< AICommandBase * >  m_commands;
    model::DefaultTimelinePtr       m_timeline;

private:

                TestAI      ( model::DefaultTimelinePtr timeline );
                ~TestAI     ();


public:

    void        EvalAt      ( TimeType t );
    std::string Repr        () const;

    void        SetTimeline ( model::DefaultTimelinePtr timeline );

private:

    void        AddCommand  ( AICommandBase * cmd );
    bool        CanBeAdded  ( AICommandBase * cmd ) const;

private:

    friend class TestAIManager;

};


} //bv
