#pragma once

#include <string>


namespace bv {

class PrintGuard
{
private:

    std::string m_leaveMessage;

public:
    
    PrintGuard      ( const std::string & message );
    PrintGuard      ( const std::string & onEnterMessage, const std::string & onLeaveMessage );

    ~PrintGuard     ();

};

} // bv
