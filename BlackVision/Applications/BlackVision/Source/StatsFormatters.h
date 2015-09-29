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


#define MAX_NAMES_SENDER_BUFFER 512


class ProfilerNamedPipeSender
{
private:
	std::unordered_map<const char*, UInt16>	m_names;
	UInt16									m_nameCounter;
	std::vector<const char*>				m_namesToSend;
	NamedPipe								m_pipe;
	bool									m_firstPipeUse;
public:
	ProfilerNamedPipeSender()
	{
		m_nameCounter = 0;
		m_firstPipeUse = true;
	}

	UInt16				GetNameID		( const char* name );
	void				SendNewNames	( unsigned int thread );

	NamedPipe&			GetNamedPipe();
};


class ProfilerDataFormatter
{
private:
	static ProfilerNamedPipeSender			s_namedPipeSender[ MAX_PROFILER_THREADS ];
public:

    static void     PrintToConsole  ( const char * msg, unsigned int thread );
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
