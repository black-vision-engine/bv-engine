#pragma once

#include <vector>

#include "System/BasicTypes.h"


namespace bv {

class AICommandBase;

class TestAI
{
private:

    std::vector< AICommandBase * >  m_commands;

private:

                TestAI      ();
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
