#pragma once

#include <memory>
#include <vector>

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Interfaces/IUpdatable.h"


namespace bv { namespace model {

class IPluginList : public IUpdatable
{
public:

    virtual IPluginConstPtr GetPlugin       ( const std::string & name ) const  = 0;
    virtual IPluginConstPtr GetPlugin       ( unsigned int idx ) const          = 0;
    virtual IPluginConstPtr GetLastPlugin   () const                            = 0;

    virtual unsigned int    NumPlugins      () const                            = 0;

    virtual                 ~IPluginList    () {}

};

DEFINE_PTR_TYPE(IPluginList)
 
} //model
} //bv
