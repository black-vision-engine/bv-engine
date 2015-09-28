#include "StatsFormatters.h"

#include <sstream>
#include <algorithm>

#include "Tools/Profiler/HerarchicalProfiler.h"
#include "FrameStatsService.h"

#include "BVConfig.h"




namespace bv
{

// *********************************
//
void    ProfilerDataFormatter::PrintToConsole  ( const char * msg, unsigned int thread )
{
	{ thread; }

    unsigned int frame = HPROFILER_GET_ACTIVE_FRAME();
    const ProfilerSample * samples = HPROFILER_GET_ONE_FRAME_SAMPLES( frame );
    unsigned int numSamples = HPROFILER_GET_NUM_SAMPLES();

    printf( "%s\n", msg );

    for( unsigned int i = 0; i < numSamples; ++i )
    {
        const ProfilerSample & sample = samples[ i ];
        const char * section = sample.type == AutoProfileType::APT_FUNCTION ? "F" : "S";

        for( unsigned int k = 0; k < sample.depth * 2; ++k )
            printf( " " );

        printf( "%s %s    %2.4f ms\n", section, sample.name, sample.durationSecs * 1000.0 );
        //printf( "%*s %s duration: %2.4f ms\n", sample.depth * 6, section, sample.name, sample.durationSecs * 1000.0 );
    }
}

// *********************************
//
void    ProfilerDataFormatter::PrintToDevNull   ( const char * msg, unsigned int thread )
{
    { msg; thread; } // FIXME: suppress unused warning
}

// *********************************
//
ProfilerNamedPipeSender ProfilerDataFormatter::s_namedPipeSender[ MAX_PROFILER_THREADS ];

UInt16 ProfilerNamedPipeSender::GetNameID		( const char* name )
{
	auto iterator = m_names.find( name );
	if( iterator != m_names.end() )
		return iterator->second;

	++m_nameCounter;
	m_names.insert( std::make_pair( name, m_nameCounter ) );
	m_namesToSend.push_back( name );
	return m_nameCounter;
}


// *********************************
//
void ProfilerNamedPipeSender::SendNames		( unsigned int thread )
{
	unsigned int numNames = (unsigned int)m_namesToSend.size();
	if( numNames > 0 )
	{
		char buffer[ sizeof( ProtocolHeader ) + MAX_NAMES_SENDER_BUFFER ];

		ProtocolHeader* header = (ProtocolHeader*)buffer;
		header->threadID = (UInt16)thread;
		header->numSamples = 0;

		//Writing string lengths to buffer.
		UInt16 stringsLegths = 0;
		UInt16* curStringLength = (UInt16*)( buffer + sizeof( ProtocolHeader ) );
		unsigned int stringNum = 0;
		for( ; stringNum < numNames && stringsLegths < MAX_NAMES_SENDER_BUFFER + sizeof( ProtocolHeader ) + sizeof( UInt32 ) *( stringNum + 1 ); stringNum++ )
		{
			*curStringLength = (UInt16)strlen( m_namesToSend[ stringNum ] );
			stringsLegths += *curStringLength;

			curStringLength++;
		}
		header->numNameStrings = (UInt16)stringNum;

		// Writing string IDs to buffer.
		UInt16* curStringID = curStringLength;
		for( unsigned int i = 0; i < stringNum; ++i )
			curStringID[ i ] = GetNameID( m_namesToSend[ i ] );
		curStringID += stringNum;

		// Writing string to buffer
		char* stringPtr = (char*)curStringID;
		for( unsigned int i = 0; i < stringNum; ++i )
		{
			int length = (UInt16)strlen( m_namesToSend[ i ] );
			memmove( stringPtr, m_namesToSend[ i ], length );
			stringPtr += length;
		}
	}
}

// *********************************
//
NamedPipe& ProfilerNamedPipeSender::GetNamedPipe()
{
	if( m_firstPipeUse )
	{
		m_pipe.ConnectToNamedPipe( std::wstring( L"ProfilerPipeTest" ), NamedPipeAccess::PipeWrite );
		m_firstPipeUse = false;
	}
	return m_pipe;
}



// *********************************
//
/**Just for now only for first thread.*/
void	ProfilerDataFormatter::SendToExternApp	( const char * msg, unsigned int thread )
{
	{ msg; } // FIXME: suppress unused warning
	///@fixme add multithreading to named pipe.
	NamedPipe& pipe = s_namedPipeSender[ thread ].GetNamedPipe();

	CPUThreadSamples* CPUsamples =  AutoProfile::GetCPUThreadSamples( thread );
	char buffer[ sizeof( ProtocolHeader ) + MAX_PROFILER_SAMPLES * sizeof( ProfilerSample ) ];

	LARGE_INTEGER freq = AutoProfile::QueryCounterFrequency();
	double freqd = (double) freq.QuadPart;

	unsigned int activeFrame = CPUsamples->m_activeFrame;
	unsigned int framesToSend = CPUsamples->m_framesToSend;
	if( framesToSend >= MAX_PROFILER_FRAMES )
		framesToSend = MAX_PROFILER_FRAMES - 1;

	//Sends all frames from first, that wasn't send yet, to active frame.
	for( ; framesToSend > 0; framesToSend-- )
	{
		int frame = activeFrame - framesToSend + 1;
		if( frame < 0 )
			frame += MAX_PROFILER_FRAMES - 1;

		ProtocolHeader* header = (ProtocolHeader*)buffer;
		header->threadID = (UInt16)thread;
		header->numSamples = (UInt16)CPUsamples->m_numFrameSamples[ frame ];
		header->numNameStrings = 0;
		header->unused = 0;

		unsigned int numLiveSamples = CPUsamples->m_numFrameSamples[ frame ];
		assert( numLiveSamples <= MAX_PROFILER_SAMPLES );

		ProfilerLiveSample* liveSamples = &CPUsamples->m_liveSamples[ frame * MAX_PROFILER_SAMPLES ];
		ProtocolSample* samples = (ProtocolSample*)( buffer + sizeof( header ) );

		for( unsigned int i = 0, k = 0; i < numLiveSamples; ++i )
		{
			ProfilerLiveSample & liveSample = liveSamples[ i ];
			ProtocolSample & sample = samples[ k ];

			sample.depth = (UInt16)liveSample.depth;
			sample.name = s_namedPipeSender[ thread ].GetNameID( liveSample.name );

			double duration = double( liveSample.timeEnd.QuadPart - liveSample.timeStart.QuadPart );
			sample.durationSecs = float( duration / freqd );

			++k;
		}

		pipe.WriteToPipe( (const char*)buffer, sizeof( ProtocolHeader ) + sizeof( ProtocolSample ) * header->numSamples );
	}

	s_namedPipeSender[ thread ].SendNames( thread );
}

// *********************************
//
void    FrameStatsFormatter::PrintToConsole     ( const FrameStatsCalculator & fsc, const char * name, unsigned int nameLen )
{
    unsigned int minFrame = 0;
    unsigned int maxFrame = 0;

    double minDuration = fsc.MinVal( name, &minFrame ) * 1000.0;
    double maxDuration = fsc.MaxVal( name, &maxFrame ) * 1000.0;
    double avg = fsc.ExpectedValue( name ) * 1000.0;
    double dev = sqrt( fsc.Variance( name ) ) * 1000.0;

    printf( "%*s: Min[%6d] %3.4f ms Max[%6d] %3.4f ms (%3.4f ms / %3.4f ms)\n", nameLen, name, minFrame, minDuration, maxFrame, maxDuration, avg, dev );
}

// *********************************
//
void    FrameStatsFormatter::PrintToConsole     ( const FrameStatsCalculator & fsc )
{
    unsigned int maxLen = 0;
    for( auto name : fsc.RegisteredSections() )
    {
        maxLen = std::max( maxLen, (unsigned int)strlen( name ) );
    }

    PrintToConsole( fsc, DefaultConfig.FrameStatsSection(), maxLen );
    for( auto name : fsc.RegisteredSections() )
    {
        if( name != DefaultConfig.FrameStatsSection() )
            PrintToConsole( fsc, name, maxLen );
    }

    unsigned int minFrame = 0;
    unsigned int maxFrame = 0;

    fsc.MinVal( DefaultConfig.FrameStatsSection(), &minFrame );
    fsc.MaxVal( DefaultConfig.FrameStatsSection(), &maxFrame );

    PrintFrameStatsToConsole( minFrame, fsc, "MinFrame", maxLen );
    PrintFrameStatsToConsole( maxFrame, fsc, "MaxFrame", maxLen );
}

// *********************************
//
std::wstring FrameStatsFormatter::FPSStatsLine  ( const FrameStatsCalculator & fsc )
{
    const char * fnm = DefaultConfig.FrameStatsSection();

    auto sample = fsc.RecentSample( fnm );

    double lastDuration = sample.duration * 1000.f;
    double avgDuration = fsc.ExpectedValue( fnm ) * 1000.0;
    double dev = sqrt( fsc.Variance( fnm ) * 1000.0 * 1000.0 );

    unsigned int minFrame = 0;
    unsigned int maxFrame = 0;
    double minDuration = fsc.MinVal( fnm, &minFrame ) * 1000.0;
    double maxDuration = fsc.MaxVal( fnm, &maxFrame ) * 1000.0;

    unsigned long numSamples = fsc.WindowSize();

    std::ostringstream  s;

    s.precision( 4 );
    s << "FPS: " << 1000.0 / lastDuration << " frame time: " << lastDuration << " ms | Stats[" << numSamples << "]->";
    s << " FPS: " << 1000.0 / avgDuration << " dura - avg: " << avgDuration << " ms dev: " << dev;
    s << " ms min: " << minDuration << " ms at " << minFrame << " max " << maxDuration <<  " ms at " << maxFrame << std::endl;

    std::string ss = s.str();
    std::wstring stemp = std::wstring( ss.begin(), ss.end() );

    return stemp;
}

// *********************************
//
void  FrameStatsFormatter::PrintFrameStatsToConsole( unsigned int frame, const FrameStatsCalculator & fsc, const char * sectionName, unsigned int frameLen )
{
    auto stats = fsc.FrameStats( frame );

    printf( "Frame stats for frame %d - %s: ", frame, sectionName );

    for( auto stat : stats )
    {
        if( stat.first == DefaultConfig.FrameStatsSection() )
            printf( "%3.4f ms \n", stat.second.duration * 1000.0 );
    }

    for( auto stat : stats )
    {
        if( stat.first != DefaultConfig.FrameStatsSection() )
            printf( "    %*s: %3.4f ms\n", frameLen, stat.first, stat.second.duration * 1000.0 );
    }
}

} //bv
