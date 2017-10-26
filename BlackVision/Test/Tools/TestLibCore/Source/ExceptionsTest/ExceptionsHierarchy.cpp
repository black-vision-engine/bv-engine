#include "ExceptionsHierarchy.h"




namespace bv
{


ExceptionType        SmallException::type       = Exception::RegisterType();
ExceptionType        BigException::type         = Exception::RegisterType();
ExceptionType        ChildishException::type    = Exception::RegisterType();



}	// bv

