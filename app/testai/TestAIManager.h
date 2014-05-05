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

    std::vector< TestAI * > m_presets;

private:


        TestAIManager   ();
        ~TestAIManager  ();

public:

    unsigned int    NumAIPresets        ();

    TestAI *        GetAIPreset         ( unsigned int idx, model::DefaultTimeline * timeline );

private:

    TestAI *        PreparePreset       ( unsigned int idx, model::DefaultTimeline * timeline );
    
    TestAI *        PreparePreset0      ( model::DefaultTimeline * timeline );
    TestAI *        PreparePreset1      ( model::DefaultTimeline * timeline );
    TestAI *        PreparePreset2      ( model::DefaultTimeline * timeline );
    TestAI *        PreparePreset3      ( model::DefaultTimeline * timeline );
    TestAI *        PreparePreset4      ( model::DefaultTimeline * timeline );
    TestAI *        PreparePreset5      ( model::DefaultTimeline * timeline );

public:

    static TestAIManager &  Instance    ();

};


} //bv

