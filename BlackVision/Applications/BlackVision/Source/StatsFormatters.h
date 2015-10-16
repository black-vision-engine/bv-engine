#pragma once

#include <string>
#include <unordered_map>

#include "Tools/Profiler/HerarchicalProfiler.h"
#include "IO/NamedPipe.h"

namespace bv
{
struct ProtocolHeader
{
	UInt16		threadID;
	UInt16		numSamples;
	UInt16		numNameStrings;
	UInt16		unused;
};

struct ProtocolSample
{
	UInt16				name;
	UInt16				depth;
	float				durationSecs;
};


#define MAX_NAMES_SENDER_BUFFER 256


/**This class sends samples to named pipe and stores functions/sections
names and identifiers.

Max number of threads shouldn't be greater than 15.
Max number of functions/sections names shouldn't be greater than 4095.
Max name string length can't be greater than MAX_NAMES_SENDER_BUFFER. Otherwise all names
that come after it, won't be send.*/
class ProfilerNamedPipeSender
{
private:
	static std::wstring						s_pipeName;

	std::unordered_map<const char*, UInt16>	m_names;
	UInt16									m_nameCounter;
	std::vector<const char*>				m_namesToSend;

	UInt16									m_thread;

	NamedPipe								m_pipe;
	bool									m_firstPipeUse;
public:
	ProfilerNamedPipeSender( UInt16 thread )
	{
		m_thread = thread;
		m_nameCounter = 0;
		m_firstPipeUse = true;
	}

	void				SendNewNames	();
	void				SendSamples		();

	// Use before you send samples for the first time.
	static void			SetNamedPipeName( const std::wstring& name ) { s_pipeName = name; }

private:
	UInt16				GetNameID		( const char* name );
	NamedPipe&			GetNamedPipe();
};


class ProfilerDataFormatter
{
private:
	static ProfilerNamedPipeSender			s_namedPipeSender[ MAX_PROFILER_THREADS ];
public:

    //static void     PrintToConsole  ( const char * msg, unsigned int thread );
    static void     PrintToDevNull  ( const char * msg, unsigned int thread );
	static void		SendToExternApp	( const char * msg, unsigned int thread );

};

class FrameStatsFormatter
{
public:

    static void         PrintToConsole          ( const class FrameStatsCalculator & fsc, const char * name, unsigned int nameLen );
    static void         PrintToConsole          ( const class FrameStatsCalculator & fsc );

    static std::wstring FPSStatsLine            ( const class FrameStatsCalculator & fsc );

    static void         PrintFrameStatsToConsole( unsigned int frame, const class FrameStatsCalculator & fsc, const char * sectionName, unsigned int frameLen );
};


} //bv
