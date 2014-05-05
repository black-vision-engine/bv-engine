#pragma once

#include <vector>

#include "System/BasicTypes.h"


namespace bv {

class AICommandBase;

namespace model {
class DefaultTimeline;
}

class TestAI
{
private:

    std::vector< AICommandBase * >  m_commands;
    model::DefaultTimeline *        m_timeline;

private:

                TestAI      ( model::DefaultTimeline * timeline );
                ~TestAI     ();


public:

    void        EvalAt      ( TimeType t );
    std::string Repr        () const;

private:

    void        AddCommand  ( AICommandBase * cmd );
    bool        CanBeAdded  ( AICommandBase * cmd ) const;

private:

    friend class TestAIManager;

};


} //bv
