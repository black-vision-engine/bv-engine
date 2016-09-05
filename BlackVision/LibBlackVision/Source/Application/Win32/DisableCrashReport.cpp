#include "stdafx.h"
#include "DisableCrashReport.h"

#include <Windows.h>


// ***********************
//
bool        IsDisableCrashReportFlagSet ( const std::wstring & cmdLineArgs )
{
	if( cmdLineArgs.find( L"-DisableCrashReport" ) != std::wstring::npos )
		return true;
	return false;
}

// ***********************
//
void        DisableCrashReport          ()
{
    SetErrorMode( SEM_NOGPFAULTERRORBOX | SEM_FAILCRITICALERRORS );
    _set_abort_behavior( 0, _WRITE_ABORT_MSG);

}


#include "Memory/MemoryLeaks.h"




