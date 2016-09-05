#include "stdafx.h"

#include "ProcessThreaded.h"




#include "Memory/MemoryLeaks.h"



namespace bv
{

// *******************************
//
ProcessThreaded::ProcessThreaded     ()
{
}

// *******************************
//
ProcessThreaded::~ProcessThreaded    ()
{
}

// *******************************
//
void ProcessThreaded::OnUpdate       ( unsigned long millis )
{
    { millis; } // FIXME: suppress unused warning
    //Intentionally left empty
}

} //bv
