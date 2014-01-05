#include "PluginParameters.h"


namespace bv {

// *******************************
//
BaseParametersDescriptor::BaseParametersDescriptor    ( )
{
}

// *******************************
// FIXME: Add validation
bool                                                    BaseParametersDescriptor::ValidateParameters          ( const std::string& programSource )    const 
{ 
    return true; 
} 

// *******************************
//
BaseParametersDescriptor::~BaseParametersDescriptor   ()
{
}

}

