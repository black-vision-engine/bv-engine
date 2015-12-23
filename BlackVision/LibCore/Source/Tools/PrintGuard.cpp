#include "PrintGuard.h"

#include <iostream>


namespace bv {

// *********************************
//
PrintGuard::PrintGuard      ( const std::string & onEnterMessage, const std::string & onLeaveMessage )
    : m_leaveMessage( onLeaveMessage )
{
    std::cout << "Enter: " << onEnterMessage << std::endl;
}

// *********************************
//
PrintGuard::~PrintGuard     ()
{
    std::cout << "Leave: " << m_leaveMessage << std::endl;
}

} // bv
