#pragma once

#include "IPlugin.h"
#include "IParameter.h"

namespace bv {

namespace model
{
    class IGeometryChannel;
}

template< class Iface, class ParameterDescriptor >
class BasePlugin : public Iface
{
protected:

    typedef ParameterDescriptor ParamDesc;

protected:

    ParamDesc                       m_paramDesc;

    std::vector< IParameter * >     m_params;
    std::vector< IValue * >         m_values;

    ///////////////// Channels //////////////////

    model::IGeometryChannel*        m_geomChannel;

public:

    explicit                                    BasePlugin          ();

    const std::string &                         GetName             () const    { return ParamDesc::pluginName; } 

    virtual const std::vector< IValue * > &     GetValuesList       () const    { return m_values; }
    virtual const std::vector< IParameter * > & GetParametersList   () const    { return m_params; };

    void                                        RegisterValue       ( IValue * v ) { m_values.push_back( v ); }


    void                                        SetGeometryChannel  (model::IGeometryChannel* geomChannel) { m_geomChannel = geomChannel; }
    const model::IGeometryChannel*              GetGeometryChannel  () const    { return m_geomChannel; }

protected:

    const ParamDesc &                           PluginParamDesc     () const { return m_paramDesc; }

};

// Implementation
template<class Iface, class ParameterDescriptor >
BasePlugin< Iface, ParameterDescriptor >::BasePlugin()
    : m_paramDesc( ParameterDescriptor() )
{}

} // bv
