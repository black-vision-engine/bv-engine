#pragma once

#include <string>



// ***********************
// Functions are used to disable pop up window that apears after crash (Debug/Send to Microsoft).
// This window needs user input to continue. Black Vision tester application must disable this functionality.


bool        IsDisableCrashReportFlagSet ( const std::wstring & cmdLineArgs );
void        DisableCrashReport          ();

