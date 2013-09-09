#pragma once

#include "IPlugin.h"
#include "IParameter.h"

namespace bv {

template< class Iface >
class BasePlugin : public Iface
{
protected:
    std::string                         m_pluginName;
    std::vector<IParameter*>            m_params;
    std::vector<IValue*>                m_values;

public:
    std::string              getName() const { return m_pluginName;} 

    virtual const std::vector<IValue*>&         GetValuesList       () const    { return m_values; }
    virtual const std::vector<IParameter*>&     GetParametersList   () const    { return m_params; };

    void                        RegisterValue(IValue* v) { m_values.push_back(v); }

    explicit BasePlugin(const std::string& name);
};

// Implementation

template<class Iface>
BasePlugin<Iface>::BasePlugin(const std::string& name)
    : m_pluginName(name)
{}

} // bv