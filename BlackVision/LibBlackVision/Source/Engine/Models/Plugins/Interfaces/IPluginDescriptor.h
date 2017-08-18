#pragma once

#include <string>

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"
#include "Engine/Models/Plugins/Interfaces/IPlugin.h"

namespace bv { namespace model {

class IPluginDescriptor
{
public:

    virtual const std::string &     GetPluginTypeUID    () const                                                                                = 0;
    virtual const std::string &     DefaultPluginName   () const                                                                                = 0;
    virtual const std::string &     GetPluginTypeAbbrv  () const                                                                                = 0;

    virtual IPluginPtr              CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const    = 0;
    virtual IPluginParamValModelPtr CreateModel         ( ITimeEvaluatorPtr timeEvaluator ) const                                               = 0;

    virtual std::string                 QueryGizmoLogicName ( const std::string & functionalityName ) const                                     = 0;
    virtual std::vector< std::string >  ListGizmoFunctions  () const                                                                            = 0;

    virtual ~IPluginDescriptor() {}

};

} //model
} //bv
