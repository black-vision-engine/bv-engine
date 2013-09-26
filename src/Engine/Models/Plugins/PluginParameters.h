#pragma once

#include "Engine/Models/Plugins/Interfaces/IPlugin.h"
#include "Engine/Types/Enums.h"

#include <hash_map>

namespace bv
{

class IParametersDescriptor
{
public:

    typedef std::hash_map<std::string, ParamType>  StringParamTypeMap;

public:

    //FIXME: add category and other required fileds here (some dependencies and so on)
    virtual const std::string &         GetPluginName           ()                                      const = 0;
    virtual const StringParamTypeMap &  GetParams               ()                                      const = 0;
    virtual bool                        ValidateParameters      ( const std::string & programSource )   const = 0;

    virtual                             ~IParametersDescriptor  (){}

};

class BaseParametersDescriptor : public IParametersDescriptor
{
protected:

    StringParamTypeMap                  m_params;
    std::string                         m_pluginName;

public:

    explicit                            BaseParametersDescriptor    ( const std::string & pluginName );

    virtual const std::string &         GetPluginName               ()                                      const;
    virtual const StringParamTypeMap &  GetParams                   ()                                      const;
    virtual bool                        ValidateParameters          ( const std::string& programSource )    const; //FIXME: Add validation

    virtual                             ~BaseParametersDescriptor   ();

};

}
