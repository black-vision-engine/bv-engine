#pragma once

#include <vector>

#include "testai/TestAI.h"

namespace bv {

class TestAI;

namespace model {
class DefaultTimeline;
}

class TestAIManager
{
private:

    std::vector< TestAI * >                 m_presets;
    std::vector< model::DefaultTimeline * > m_timelines;

private:


        TestAIManager   ();
        ~TestAIManager  ();

public:

    unsigned int    NumAIPresets        () const;

    TestAI *        GetAIPreset         ( unsigned int idx );

private:

    TestAI *        PreparePreset       ( unsigned int idx ) const;
    
    TestAI *        PreparePreset0      () const;
    TestAI *        PreparePreset1      () const;
    TestAI *        PreparePreset2      () const;
    TestAI *        PreparePreset3      () const;
    TestAI *        PreparePreset4      () const;
    TestAI *        PreparePreset5      () const;

public:

    static TestAIManager &  Instance    ();

};


} //bv

