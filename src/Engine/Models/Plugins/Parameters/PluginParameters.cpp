#include "PluginParameters.h"


namespace bv {

// *******************************
//
BaseParametersDescriptor::BaseParametersDescriptor    ( const std::string & pluginName )
    : m_pluginName( pluginName )
{
}

// *******************************
//
const std::string &                                     BaseParametersDescriptor::GetPluginName               () const
{
    return m_pluginName;
}

// *******************************
//
const BaseParametersDescriptor::StringParamTypeMap &    BaseParametersDescriptor::GetParams                   () const 
{ 
    return m_params; 
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

