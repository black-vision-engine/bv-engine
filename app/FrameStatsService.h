#include <queue>
#include <hash_map>

#include "System/HRTimer.h"


namespace bv
{

struct MovingAverageStatsHasher;

struct FrameStatsSample
{
    unsigned int    frame;
    double          duration;

    LARGE_INTEGER   startTime;
};

struct MovingAverageData
{
    std::queue< FrameStatsSample >  samples;

    double          minDuration;
    unsigned int    minDurationFrame;

    double          maxDuration;
    unsigned int    maxDurationFrame;

    double          accumDuration;
    double          accumDurationSquares;

public:

            MovingAverageData   ( unsigned int numSamples );

private:

    void    Initialize          ( unsigned int numSamples );

    void    ResetAccumStats     ();

    void    RecalculateStats    ();
    void    AccumulateSample    ( const FrameStatsSample & sample );

    double  ExpectedValue       () const;
    double  Variance            () const;
    double  MinVal              ( unsigned int * frame = nullptr ) const;
    double  MaxVal              ( unsigned int * frame = nullptr ) const;

    inline void AddNextSample   ( const FrameStatsSample & sample );

    friend class FrameStatsCalculator;

};

class FrameStatsCalculator
{
private:

    typedef std::hash_map< const char *, MovingAverageData * >  TSamplersMap;
    typedef std::hash_map< const char *, FrameStatsSample >     TSingleSamplesMap;

private:

    unsigned int        m_windowSize;

    unsigned int        m_frame;

    TSamplersMap        m_samplers;
    TSingleSamplesMap   m_stateBuffer;

    HighResolutionTimer m_timer;

public:

            FrameStatsCalculator    ( unsigned int movingAverageWindowSize );
            ~FrameStatsCalculator   ();

    unsigned int    CurFrame        () const;
    unsigned int    WindowSize      () const;

    unsigned int    NextFrame       ();
    void            ResetTimer      ();

    inline void StartSection        ( const char * name );
    inline void EndSection          ( const char * name );

    void        RecalculateStats    ();

    double      ExpectedValue       ( const char * name ) const;
    double      Variance            ( const char * name ) const;
    double      MinVal              ( const char * name, unsigned int * frame = nullptr ) const;
    double      MaxVal              ( const char * name, unsigned int * frame = nullptr ) const;

    FrameStatsSample RecentSample   ( const char * name ) const;

};

class FrameStatsFrameScope
{
private:

    FrameStatsCalculator * m_calc;

public:

    inline FrameStatsFrameScope     ( FrameStatsCalculator * fsc )
        : m_calc( fsc )
    {
        fsc->ResetTimer();
    }

    inline ~FrameStatsFrameScope    ()
    {
        m_calc->NextFrame();
    }

};

class FrameStatsSectionScope
{
private:

    FrameStatsCalculator *  m_calc;
    const char *            m_sectionName;

public:

    inline FrameStatsSectionScope     ( FrameStatsCalculator * fsc, const char * sectionName )
        : m_calc( fsc )
        , m_sectionName( sectionName )
    {
        fsc->StartSection( sectionName );
    }

    inline ~FrameStatsSectionScope    ()
    {
        m_calc->EndSection( m_sectionName );
    }
};

} //bv

#include "FrameStatsService.inl"
