#include "stdafx.h"

#include "ScopedCriticalSection.h"


namespace bv 
{

// *************************************
//
ScopedCriticalSection::ScopedCriticalSection   ( CriticalSection &  criticalSection )
    : m_criticalSection( criticalSection )
{
    m_criticalSection.Lock();
}
  
// *************************************
//
ScopedCriticalSection::~ScopedCriticalSection()
{
    m_criticalSection.Unlock();
}

} //bv
