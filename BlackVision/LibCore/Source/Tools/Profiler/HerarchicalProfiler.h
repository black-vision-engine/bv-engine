#pragma once

#include <windows.h>

#include "CoreDEF.h"
#include <climits>

#define MAX_PROFILER_SAMPLES		120
#define MAX_PROFILER_FRAMES			10
#define MAX_PROFILER_THREADS		1

#define PROFILER_THREAD1			0
#define PROFILER_THREAD2			1
#define PROFILER_THREAD3			2
#define PROFILER_THREAD4			3
#define PROFILER_THREAD5			4
#define PROFILER_THREAD6			5

namespace bv
{

const int64_t INVALID_TIME = LLONG_MIN;

enum class AutoProfileType : int
{
    APT_FUNCTION = 0,
    APT_SECTION
};


struct ProtocolHeader
{
	UInt16		threadID;
	UInt16		numSamples;
	UInt16		numNameStrings;
};


struct ProfilerSample
{
    const char *        name;

    LARGE_INTEGER       duration;
    double              durationSecs;

    AutoProfileType     type;
    unsigned int        depth;

};

struct ProfilerLiveSample
{
    const char *        name;
   
    LARGE_INTEGER       timeStart;
	LARGE_INTEGER		timeEnd;
    AutoProfileType     type;
	unsigned int		depth;

	ProfilerLiveSample()
	{
		timeStart.QuadPart = INVALID_TIME;
		timeEnd.QuadPart = INVALID_TIME;
		name = nullptr;
		depth = 0;
	}
};


struct CPUThreadSamples
{
	ProfilerLiveSample			m_liveSamples[ MAX_PROFILER_SAMPLES * MAX_PROFILER_FRAMES ];
	unsigned int				m_numFrameSamples[ MAX_PROFILER_FRAMES ];
	
	unsigned int				m_curDepth;
	unsigned int				m_curSample;
	unsigned int				m_curFrame;
	unsigned int				m_activeFrame;
	unsigned int				m_framesToSend;

	CPUThreadSamples()
	{
		for( auto& numSamples : m_numFrameSamples )
			numSamples = 0;
		m_curSample = 0;
		m_curDepth = 0;
		m_curFrame = 0;
		m_activeFrame = 0;
		m_framesToSend = 0;
	}
};

class AutoProfile
{
private:
	static CPUThreadSamples		m_threads[ MAX_PROFILER_THREADS ];

	unsigned int				m_threadID;
    static unsigned int         m_displayStatsWaitMillis;

	// @todo Zlikwidowaæ. To jest potrzebne na razie tylko po to, ¿eby wypisywaæ dane na konsolê.
    static ProfilerSample       m_samples[ MAX_PROFILER_SAMPLES * MAX_PROFILER_FRAMES ];
public:

    inline                  AutoProfile             ( const char * name, AutoProfileType type, unsigned int threadID = 0 );
    inline                  ~AutoProfile            ();

private:

    static  void            StartFrame              ( unsigned int threadID = 0 );
    static  void            EndFrame                ( unsigned int threadID = 0 );

public:

    static void             SetStatsDisplayWaitMs   ( unsigned int millis );
    static unsigned int     GetStatsDisplayWaitMs   ();

    static unsigned int     NumSamples              ( unsigned int threadID = 0 );
    static unsigned int     NumFrames               ( unsigned int threadID = 0 );
    static unsigned int     ActiveFrame             ( unsigned int threadID = 0 );

    static LARGE_INTEGER    QueryCounterFrequency   ();

	static CPUThreadSamples*	GetCPUThreadSamples			( unsigned int thread );
    static const ProfilerSample *   OneFrameSamples				( unsigned int frame );
    static const ProfilerSample *   AveragedSamples				();

    friend class AutoFrameProfile;
};

class AutoFrameProfile
{
public:

    typedef void (*PtrDisplayCallback)( const char * );

private:
	unsigned int				m_threadID;
    static bool                 m_showStats;
    static PtrDisplayCallback   m_displayCallback;

public:

    inline  AutoFrameProfile    ( unsigned int threadID = 0 );
    inline  ~AutoFrameProfile   ();

    static void    SetDisplayStats          ();
    static void    RegisterDisplayCallback  ( PtrDisplayCallback callback );

};

} //bv

#define HPROFILER_NEW_FRAME( thread )               AutoFrameProfile COMBINE(frame_sample_,__LINE__) ( thread )

#define HPROFILER_FUNCTION( name, thread )          AutoProfile COMBINE(function_sample_,__LINE__) ( name, AutoProfileType::APT_FUNCTION, thread )
#define HPROFILER_SECTION( name, thread )           AutoProfile COMBINE(section_sample_,__LINE__) ( name, AutoProfileType::APT_SECTION, thread )

#define HPROFILER_GET_ONE_FRAME_SAMPLES( frame )    AutoProfile::OneFrameSamples( frame )
#define HPROFILER_GET_AVERAGED_SAMPLES()            AutoProfile::AveragedSamples()

#define HPROFILER_GET_NUM_SAMPLES()                 AutoProfile::NumSamples()
#define HPROFILER_GET_NUM_FRAMES()                  AutoProfile::NumFrames()
#define HPROFILER_GET_ACTIVE_FRAME()                AutoProfile::ActiveFrame()

#define HPROFILER_SET_DISPLAY_WAIT_MILLIS( millis ) AutoProfile::SetStatsDisplayWaitMs( millis )
#define HPROFILER_REGISTER_DISPLAY_CALLBACK( cb )   AutoFrameProfile::RegisterDisplayCallback( cb )
#define HPROFILER_SET_FORCED_DISPLAY()              AutoFrameProfile::SetDisplayStats()

#include "HerarchicalProfiler.inl"
