#pragma once

#include <vector>

#include "testai/TestAI.h"
#include "Engine/Models/Interfaces/IModelNode.h"

namespace bv {

class TestAI;
class BVAppLogic;

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

    TestAI *        GetAIPreset         ( unsigned int idx, model::IModelNodePtr node = nullptr );
    TestAI *        GetAIPreset         ( unsigned int idx, BVAppLogic * logic );


private:

    TestAI *        PreparePreset       ( unsigned int idx, model::IModelNodePtr node = nullptr ) const;
    TestAI *        PreparePreset       ( unsigned int idx, BVAppLogic * logic ) const;
    
    TestAI *        PreparePreset0      () const;
    TestAI *        PreparePreset1      () const;
    TestAI *        PreparePreset2      () const;
    TestAI *        PreparePreset3      ( model::IModelNodePtr node ) const;
    TestAI *        PreparePreset4      ( BVAppLogic * logic ) const;
    TestAI *        PreparePreset5      ( BVAppLogic * logic ) const;
    TestAI *        PreparePreset6      ( model::IModelNodePtr node ) const;


public:

    static TestAIManager &  Instance    ();

};


} //bv

/*
        static float ts[] = { 1.0f, 2.f, 5.f, 10.f, 12.f, 15.f, 18.f, 22.f, 24.f, 30.f, 35.f };

        static model::TimeSegmentEvalImpl te( 5.0f );

        te.UpdateGlobalTime( t );
        printf( "\rGT: %3.3f LT: %3.3f", t, te.GetLocalTime() );

        if( t >= ts[ 0 ] )
        {
            te.Start();
            printf( "  ACTION: %3.3f -> Start\n", t );
            te.SetWrapBehaviorPost( TimelineWrapMethod::TWM_MIRROR );
            printf( "  ACTION: %3.3f -> SetWrapBehaviorPost( MIRROR )\n", t );
            te.SetWrapBehaviorPre( TimelineWrapMethod::TWM_MIRROR );
            printf( "  ACTION: %3.3f -> SetWrapBehaviorPre( MIRROR )\n", t );
            ts[ 0 ] = 10000.0f;
        }
        else if( t >= ts[ 1 ] )
        {
            te.Reverse();
            printf( "  ACTION: %3.3f -> Reverse\n", t );
            ts[ 1 ] = 10000.0f;
        }
        else if( t >= ts[ 2 ] )
        {
            te.Stop();
            printf( "  ACTION: %3.3f -> Stop\n", t );
            ts[ 2 ] = 10000.0f;
        }
        else if( t >= ts[ 3 ] )
        {
            te.Start();
            printf( "  ACTION: %3.3f -> Start\n", t );
            ts[ 3 ] = 10000.0f;
        }
        else if( t >= ts[ 4 ] )
        {
            te.Stop();
            printf( "  ACTION: %3.3f -> Stop\n", t );
            ts[ 4 ] = 10000.0f;
        }
        else if( t >= ts[ 5 ] )
        {
            te.Reverse();
            printf( "  ACTION: %3.3f -> Reverse\n", t );
            ts[ 5 ] = 10000.0f;
        }
        else if( t >= ts[ 6 ] )
        {
            te.Start();
            printf( "  ACTION: %3.3f -> Start\n", t );
            ts[ 6 ] = 10000.0f;
        }
*/
