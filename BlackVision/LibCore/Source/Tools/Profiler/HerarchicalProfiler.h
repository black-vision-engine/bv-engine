#pragma once


#include "CoreDEF.h"
#include <climits>

#define MAX_PROFILER_SAMPLES		400
#define MAX_PROFILER_FRAMES			10
#define MAX_PROFILER_THREADS		1

#define PROFILER_THREAD1			0
#define PROFILER_THREAD2			1
#define PROFILER_THREAD3			2
#define PROFILER_THREAD4			3
#define PROFILER_THREAD5			4
#define PROFILER_THREAD6			5


/**
@section HerarchicalProfiler - instruction

##Simple use case
Profiler uses extern application to present data from profiler.
EditorProfiler launches BlackVision executable and initializes profiler from
command line parameter (see bvApp.cpp InitializeAppLogic).

To profile part of code call HPROFILER_NEW_FRAME on the begining of each frame.

Use macro HPROFILER_FUNCTION or HPROFILER_SECTION to profile code section or function
and specify name of section and thread number which executes this part of code.

@attention Profiler doesn't copy name string from parameter. It only saves pointer for itself.
You have to ensure that this pointer still exists, when profiler sends informations to external app.

@note To see max number of names take a look at ProfilerNamedPipeSender in file StatsFormatters.h (project BlackVision).

@code
void BlackVisionApp::OnIdle		()
{
    HPROFILER_NEW_FRAME( PROFILER_THREAD1 );
    HPROFILER_FUNCTION( "BlackVisionApp::OnIdle", PROFILER_THREAD1 );
	// code1

	HPROFILER_FUNCTION( "BVAppLogic::OnUpdate", PROFILER_THREAD1 );
	// code2

}
@endcode

HPROFILER_FUNCTION / HPROFILER_SECTION creates an object on stack which messures time
from beginning of its live, to the moment when it is deleted.
In this example both HPROFILER_FUNCTIONs will be deleted on the end of function.

"BlackVisionApp::OnIdle" will messure time of executing code1 + code2
and "BVAppLogic::OnUpdate" will messure time of executing only code2.

When you want to profile only part of function, you can use {} like this

@code 
void BlackVisionApp::OnIdle		()
{
    HPROFILER_NEW_FRAME( PROFILER_THREAD1 );

	{
    HPROFILER_FUNCTION( "BlackVisionApp::OnIdle", PROFILER_THREAD1 );
	
	// code1
	
	}

	{
	HPROFILER_FUNCTION( "BVAppLogic::OnUpdate", PROFILER_THREAD1 );
	
	// code2

	}
}
@endcode

In this example "BlackVisionApp::OnIdle" will messure only code1 execution time.

##Multithreading
To avoid synchronization, profiler have different dataset for every thread. It doesn't use mutex or semaphores.
You have to specify explicitly, which thread executes profilers functions, otherwise unexpected errors can occur.

Use macros PROFILER_THREAD1 - PROFILER_THREAD6. Remember to change MAX_PROFILER_THREADS value.

Theese macros can be used not only for different threads. You can set them in one thread
to have different hierarchies of samples.


##Profiler modes
There're three profiler modes:
- ProfilerMode::PM_EVERY_FRAME							- Samples are sent every frame.
- ProfilerMode::PM_EVERY_N_FRAMES_AND_FORCE_DISPLAY		- You can set number of frames after that samples are sent. Profiler will sent only MAX_PROFILER_FRAMES samples.
- ProfilerMode::PM_WAIT_TIME_AND_FORCE_DISPLAY			- You can set time after that samples are sent. Profiler will sent only MAX_PROFILER_FRAMES samples.

Mode can be set with macro HPROFILER_SET_DISPLAY_MODE.
Number of samples in ProfilerMode::PM_EVERY_N_FRAMES_AND_FORCE_DISPLAY can be set with macro HPROFILER_SET_DISPLAY_AFTER_NUM_FRAMES.
Time period used in mode ProfilerMode::PM_WAIT_TIME_AND_FORCE_DISPLAY can be sest with macro HPROFILER_SET_DISPLAY_WAIT_MILLIS.

##Force display
In some of theese modes not all samples are sent to external application.
Sometimes there's need to force profiler to send interesting data (for example: frame was the longest one).
Use macro HPROFILER_SET_FORCED_DISPLAY to do this.

*/


namespace bv
{

const uint64_t INVALID_TIME = std::numeric_limits< uint64_t >::max();
const std::wstring PROFILER_ENABLE_ARGUMENT_STRING = L"-EnableProfiler";		///<This string should contain main function arguments.

enum class AutoProfileType : int
{
    APT_FUNCTION = 0,
    APT_SECTION
};


enum class ProfilerMode
{
	PM_EVERY_FRAME,								///<Samples are sent every frame.
	PM_EVERY_N_FRAMES_AND_FORCE_DISPLAY,		///<You can set number of frames after that samples are sent. Profiler will sent only MAX_PROFILER_FRAMES samples.
	PM_WAIT_TIME_AND_FORCE_DISPLAY				///<You can set time after that samples are sent. Profiler will sent only MAX_PROFILER_FRAMES samples.
};


struct ProfilerLiveSample
{
    const char *        name;
   
    UInt64              timeStart;
    UInt64      		timeEnd;
    AutoProfileType     type;
	unsigned int		depth;

	ProfilerLiveSample()
	{
		timeStart = INVALID_TIME;
		timeEnd = INVALID_TIME;
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

    static UInt64               QueryCounterFrequency   ();
    static UInt64               QueryCounter            ();

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

#define HPROFILER_NEW_FRAME( thread )               ::bv::AutoFrameProfile COMBINE(frame_sample_,__LINE__) ( thread )

#define HPROFILER_FUNCTION( name, thread )          ::bv::AutoProfile COMBINE(function_sample_,__LINE__) ( name, ::bv::AutoProfileType::APT_FUNCTION, thread )
#define HPROFILER_SECTION( name, thread )           ::bv::AutoProfile COMBINE(section_sample_,__LINE__) ( name, ::bv::AutoProfileType::APT_SECTION, thread )

#define HPROFILER_SET_FORCED_DISPLAY()              ::bv::AutoFrameProfile::SetDisplayStats()
#define HPROFILER_REGISTER_DISPLAY_CALLBACK( cb )   ::bv::AutoFrameProfile::RegisterDisplayCallback( cb )

#define HPROFILER_SET_DISPLAY_AFTER_NUM_FRAMES( n )	::bv::AutoFrameProfile::SetFramesToShow( n )
#define HPROFILER_SET_DISPLAY_MODE( mode )			::bv::AutoFrameProfile::SetDisplayMode( mode )
#define HPROFILER_SET_DISPLAY_WAIT_MILLIS( millis ) ::bv::AutoProfile::SetStatsDisplayWaitMs( millis )

#include "HerarchicalProfiler.inl"
