#pragma once

#include "IPlugin.h"
#include "IParameter.h"

namespace bv {

namespace model
{
    class IGeometryChannel;
    class ITransformChannel;
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
    model::ITransformChannel*       m_transformChannel;

public:

    explicit                                    BasePlugin          ();

    const std::string &                         GetName             () const    { return ParamDesc::pluginName; } 

    virtual const std::vector< IValue * > &     GetValuesList       () const    { return m_values; }
    virtual const std::vector< IParameter * > & GetParametersList   () const    { return m_params; };

    void                                        RegisterValue       ( IValue * v ) { m_values.push_back( v ); }


    void                                        SetGeometryChannel  (model::IGeometryChannel* geomChannel)          { m_geomChannel = geomChannel; }
    void                                        SetTransformChannel (model::ITransformChannel* transformChannel)    { m_transformChannel = transformChannel; }

    const model::IGeometryChannel*              GetGeometryChannel  () const    { return m_geomChannel; }
    const model::ITransformChannel*             GetTransformChannel () const    { return m_transformChannel; }

protected:

    const ParamDesc &                           PluginParamDesc     () const { return m_paramDesc; }

};

// Implementation
template<class Iface, class ParameterDescriptor >
BasePlugin< Iface, ParameterDescriptor >::BasePlugin()
    : m_paramDesc( ParameterDescriptor() )
    , m_geomChannel( nullptr )
    , m_transformChannel(nullptr)
{}

} // bv
