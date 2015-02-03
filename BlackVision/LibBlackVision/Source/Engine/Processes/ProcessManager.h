#pragma once

#include <list>

#include "Engine/Processes/Process.h"


namespace bv
{

class ProcessManager
{
private:

    std::list<ProcessPtr>   m_processList;

public:

                    ProcessManager      ();
                    ~ProcessManager     ();

    ProcessWeakPtr  Attach              ( ProcessPtr process );

    void            Update              ( unsigned long millis );

    void            AbortAll            ( bool immediateFlag );

	SizeType        GetProcessCount     () const;

private:

	void            ClearAll            ();

};

} //bv
