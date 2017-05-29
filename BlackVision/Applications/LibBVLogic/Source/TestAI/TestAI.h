#pragma once

#include <vector>

#include "CoreDEF.h"
#include "Engine/Models/Timeline/Dynamic/DefaultTimeline.h"

#include "Engine/Models/Interfaces/IModelNode.h"


namespace bv {

class AICommandBase;

class TestAI
{
private:

    std::vector< AICommandBase * >  m_commands;
    model::DefaultTimelinePtr       m_timeline;
    model::IModelNodePtr            m_root;

private:

                TestAI      ( model::DefaultTimelinePtr timeline, model::IModelNodePtr root );
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
