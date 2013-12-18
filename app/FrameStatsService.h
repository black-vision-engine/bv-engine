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

            MovingAverageData   ();

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

    typedef std::hash_map< const char *, MovingAverageData > TSamplersMap;

private:

    unsigned int        m_windowSize;

    TSamplersMap        m_samplers;

    HighResolutionTimer m_timer;

public:

            FrameStatsCalculator    ( unsigned int movingAverageWindowSize );
            ~FrameStatsCalculator   ();

    inline void StartSection        ( const char * name, unsigned int frame );
    inline void StopSection         ( const char * name, unsigned int frame );

    void        RecalculateStats    ();

    double      ExpectedValue       ( const char * name ) const;
    double      Variance            ( const char * name ) const;
    double      MinVal              ( const char * name, unsigned int * frame = nullptr ) const;
    double      MaxVal              ( const char * name, unsigned int * frame = nullptr ) const;

private:

    void        InitializeSampler   ( const char * name );

};

} //bv

#include "FrameStatsService.inl"
