#pragma once

#include "Engine/Events/Interfaces/IEvent.h"


namespace bv
{

class MockFrameReader
{
private:

    char * m_buf;

public:

            MockFrameReader     ();
            ~MockFrameReader    ();

    void    ProcessFrame        ( IEventPtr evt );

};

} //bv
