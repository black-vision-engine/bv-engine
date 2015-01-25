#include "CriticalSection.h"


namespace bv 
{

// *************************************
//
CriticalSection::CriticalSection()
{ 
    InitializeCriticalSection( &m_criticalSection ); 
}
 
// *************************************
//
CriticalSection::~CriticalSection()
{
    DeleteCriticalSection( &m_criticalSection ); 
}

// *************************************
//
void CriticalSection::Lock()
{ 
    EnterCriticalSection( &m_criticalSection ); 
}

// *************************************
//
void CriticalSection::Unlock()
{ 
    LeaveCriticalSection( &m_criticalSection ); 
}

} //bv
