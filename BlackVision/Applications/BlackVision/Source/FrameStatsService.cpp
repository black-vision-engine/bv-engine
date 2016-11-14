#include "stdafxBVApp.h"

#include "FrameStatsService.h"

#include  <cassert>


namespace bv
{


// *********************************
//
MovingAverageData::MovingAverageData            ( unsigned int numSamples )
{
    Initialize( numSamples );
}

// *********************************
//
void MovingAverageData::Initialize              ( unsigned int numSamples )
{
    assert( samples.size() == 0 );

    ResetAccumStats();

    FrameStatsSample sample;
    sample.duration = 0.0;
    sample.frame = INVALID_FRAME;

    for( unsigned int i = 0; i < numSamples; ++i )
    {
        samples.push_back( sample );
    } 
}

// *********************************
//
void MovingAverageData::ResetAccumStats         ()
{
    minDuration             = 10000000.0;
    minDurationFrame        = INVALID_FRAME;

    maxDuration             = 0.0;
    maxDurationFrame        = INVALID_FRAME;

    accumDuration           = 0.0;
    accumDurationSquares    = 0.0;
}

// *********************************
//
void MovingAverageData::RecalculateStats        ()
{
    ResetAccumStats();

	unsigned int validSample = CountUselessSamples();
	for( ; validSample < samples.size(); ++validSample )
    {
        AccumulateSample( samples[ validSample ] );
    }
}

// *********************************
//
void    MovingAverageData::AccumulateSample     ( const FrameStatsSample & sample )
{
    accumDuration += sample.duration;
    accumDurationSquares += sample.duration * sample.duration;

    if( sample.duration < minDuration )
    {
        minDuration = sample.duration;
        minDurationFrame = sample.frame;
    }

    if( sample.duration > maxDuration )
    {
        maxDuration = sample.duration;
        maxDurationFrame = sample.frame;
    }
}

// *********************************
//
double  MovingAverageData::ExpectedValue        () const
{
	return accumDuration / ( (double) samples.size() - CountUselessSamples() );
}

// *********************************
//
double  MovingAverageData::Variance             () const
{
    double EX2 = ExpectedValue();
    EX2 *= EX2;

	return accumDurationSquares / ( (double) samples.size() - CountUselessSamples() ) - EX2;
}

// *********************************
//
double  MovingAverageData::MinVal               ( unsigned int * frame ) const
{
    if( frame )
        *frame = minDurationFrame;

    return minDuration;
}

// *********************************
//
double  MovingAverageData::MaxVal               ( unsigned int * frame ) const
{
    if( frame )
        *frame = maxDurationFrame;

    return maxDuration;
}

// *********************************
//
FrameStatsSample MovingAverageData::FrameStats  ( unsigned int frame ) const
{
    for( auto sample : samples )
    {
        if( sample.frame == frame )
            return sample;
    }

    assert( false );
    return FrameStatsSample();
}

// *********************************
//
FrameStatsCalculator::FrameStatsCalculator      ( unsigned int movingAverageWindowSize )
    : m_windowSize( movingAverageWindowSize )
    , m_frame( 0 )
{
    assert( m_windowSize > 0 );
}

// *********************************
//
FrameStatsCalculator::~FrameStatsCalculator     ()
{
    for( auto it : m_samplers )
    {
        delete it.second;
    }
}

// *********************************
//
unsigned int FrameStatsCalculator::CurFrame     () const
{
    return m_frame;
}

// *********************************
//
unsigned int FrameStatsCalculator:: WindowSize  () const
{
    return m_windowSize;
}

// *********************************
//
unsigned int FrameStatsCalculator::NextFrame    ()
{
    return m_frame++;
}

// *********************************
//
void    FrameStatsCalculator::ResetTimer        ()
{
    m_timer.Reinitialize();
}

// *********************************
//
void    FrameStatsCalculator::RecalculateStats  ()
{
    for ( auto it : m_samplers )
    {
        //const char * name = it.first;
        it.second->RecalculateStats();
    }
}

// *********************************
//
double  FrameStatsCalculator::ExpectedValue     ( const char * name ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->ExpectedValue();
}

// *********************************
//
double  FrameStatsCalculator::Variance          ( const char * name ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->Variance();
}

// *********************************
//
double  FrameStatsCalculator::MinVal            ( const char * name, unsigned int * frame ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->MinVal( frame );
}

// *********************************
//
double  FrameStatsCalculator::MaxVal            ( const char * name, unsigned int * frame ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->MaxVal( frame );
}

// *********************************
//
bool FrameStatsCalculator::WasSampledMinVal( const char * name ) const
{
    assert( m_minFlags.find( name ) != m_minFlags.end() );

    const auto it = m_minFlags.find( name );

    return it->second;
}

// *********************************
//
bool FrameStatsCalculator::WasSampledMaxVal( const char * name ) const
{
    assert( m_maxFlags.find( name ) != m_maxFlags.end() );

    const auto it = m_maxFlags.find( name );

    return it->second;
}

// *********************************
//
FrameStatsSample    FrameStatsCalculator::RecentSample( const char * name ) const
{
    assert( m_samplers.find( name ) != m_samplers.end() );

    const auto it = m_samplers.find( name );
    return it->second->samples.back();
}

// *********************************
//
FrameStatsCalculator::TSingleSamplesMap   FrameStatsCalculator::FrameStats  ( unsigned int frame ) const
{
    FrameStatsCalculator::TSingleSamplesMap retMap;

    for( auto it : m_samplers )
    {
        retMap[ it.first ] = it.second->FrameStats( frame );
    }

    return retMap;
}

// *********************************
//
const FrameStatsCalculator::TSectionsNamesVec &    FrameStatsCalculator::RegisteredSections  () const
{
    return m_sectionsNames;
}

} //bv
