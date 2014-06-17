#pragma once

#include <string>

#include "Engine/Models/Interfaces/ITimeEvaluator.h"
#include "Engine/Models/Plugins/Interfaces/IPluginParamValModel.h"


namespace bv { namespace model {

class IPlugin;

class IPluginDescriptor
{
public:

    virtual const std::string &     GetPluginTypeUID    () const                                                                                    = 0;
    virtual const std::string &     DefaultPluginName   () const                                                                                    = 0;

    virtual bool                    CanBeAttachedTo     ( const IPlugin * plugin )  const                                                           = 0;
    virtual IPlugin *               CreatePlugin        ( const std::string & name, const IPlugin * prev, ITimeEvaluatorPtr timeEvaluator ) const   = 0;
    virtual IPluginParamValModelPtr CreateModel         ( ITimeEvaluatorPtr timeEvaluator ) const                                                   = 0;

    virtual ~IPluginDescriptor() {}

};

} //model
} //bv
