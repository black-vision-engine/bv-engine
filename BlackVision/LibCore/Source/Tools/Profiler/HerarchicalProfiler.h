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
const std::wstring PROFILER_ENABLE_ARGUMENT_STRING = L"-EnableProfiler";		///<This string should contain main function arguments.

enum class AutoProfileType : int
{
    APT_FUNCTION = 0,
    APT_SECTION
};


enum class ProfilerMode
{
	PM_EVERY_FRAME,
	PM_EVERY_N_FRAMES_AND_FORCE_DISPLAY,
	PM_WAIT_TIME_AND_FORCE_DISPLAY
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
public:

    inline                  AutoProfile             ( const char * name, AutoProfileType type, unsigned int threadID = 0 );
    inline                  ~AutoProfile            ();

private:

    static  void            StartFrame              ( unsigned int threadID );
    static  void            EndFrame                ( unsigned int threadID );

public:

    static void             SetStatsDisplayWaitMs   ( unsigned int millis );
    static unsigned int     GetStatsDisplayWaitMs   ();

    static unsigned int     NumSamples              ( unsigned int threadID );
    static unsigned int     NumFrames               ( unsigned int threadID );
    static unsigned int     ActiveFrame             ( unsigned int threadID );

    static LARGE_INTEGER		QueryCounterFrequency   ();

	static CPUThreadSamples*	GetCPUThreadSamples			( unsigned int thread );

    friend class AutoFrameProfile;
};

class AutoFrameProfile
{
public:

	typedef void (*PtrDisplayCallback)( const char *, unsigned int );

private:
	unsigned int				m_threadID;
    static bool                 m_showStats;		///< Force showing data.
    static PtrDisplayCallback   m_displayCallback;
	static ProfilerMode			m_mode;
	static unsigned int			m_framesToShow;		///< Show data after this number of frames. Only for mode PM_EVERY_N_FRAMES_AND_FORCE_DISPLAY.

public:

    inline  AutoFrameProfile    ( unsigned int threadID );
    inline  ~AutoFrameProfile   ();

	static void		SetFramesToShow			 ( unsigned int frames )		{ m_framesToShow = frames; }
	static void		SetDisplayMode			 ( ProfilerMode mode )			{ m_mode = mode; }
    static void		SetDisplayStats          ();
    static void		RegisterDisplayCallback  ( PtrDisplayCallback callback );

};

inline bool IsProfilerEnabled( const std::wstring& commandLineArgs )
{
	if( commandLineArgs.find( PROFILER_ENABLE_ARGUMENT_STRING ) != std::wstring::npos )
		return true;
	return false;
}


} //bv

#define HPROFILER_NEW_FRAME( thread )               AutoFrameProfile COMBINE(frame_sample_,__LINE__) ( thread )

#define HPROFILER_FUNCTION( name, thread )          AutoProfile COMBINE(function_sample_,__LINE__) ( name, AutoProfileType::APT_FUNCTION, thread )
#define HPROFILER_SECTION( name, thread )           AutoProfile COMBINE(section_sample_,__LINE__) ( name, AutoProfileType::APT_SECTION, thread )

#define HPROFILER_SET_FORCED_DISPLAY()              AutoFrameProfile::SetDisplayStats()
#define HPROFILER_REGISTER_DISPLAY_CALLBACK( cb )   AutoFrameProfile::RegisterDisplayCallback( cb )

#define HPROFILER_SET_DISPLAY_AFTER_NUM_FRAMES( n )	AutoFrameProfile::SetFramesToShow( n )
#define HPROFILER_SET_DISPLAY_MODE( mode )			AutoFrameProfile::SetDisplayMode( mode )
#define HPROFILER_SET_DISPLAY_WAIT_MILLIS( millis ) AutoProfile::SetStatsDisplayWaitMs( millis )

#include "HerarchicalProfiler.inl"
