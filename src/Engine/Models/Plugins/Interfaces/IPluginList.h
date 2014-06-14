#pragma once

#include <memory>
#include <vector>

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Interfaces/IUpdatable.h"


namespace bv { namespace model {

class IPluginList : public IUpdatable
{
public:

    virtual const IPlugin * GetPlugin       ( const std::string & name ) const  = 0;
    virtual const IPlugin * GetPlugin       ( unsigned int idx ) const          = 0;
    virtual const IPlugin * GetLastPlugin   () const                            = 0;

    virtual unsigned int    NumPlugins      () const                            = 0;

    virtual                 ~IPluginList    () {}

};

typedef std::shared_ptr< IPluginList > IPluginListPtr;
 
} //model
} //bv
