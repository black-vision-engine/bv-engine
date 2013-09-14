#pragma once

#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugin.h"

#include "Solid.h" // FIXME: move ParametersDescriptor to separated header

namespace bv
{

class ParamFloat;
class ValueFloat;

// ***************************** DESCRIPTOR **********************************
class ExtrudeParamDescriptor : public BaseParametersDescriptor 
{
public:
    
    static const std::string pluginName;

    static const std::string scaleParamName;

    explicit ExtrudeParamDescriptor();
};


// ***************************** PLUGIN ********************************** 
class ExtrudePlugin : public BasePlugin< IShaderPlugin, ExtrudeParamDescriptor >
{
private:

    ParamFloat *                m_scaleParam;
    ValueFloat *                m_scaleValue;

public:

    explicit                ExtrudePlugin   ( const FloatInterpolator & scale );
                            ~ExtrudePlugin  ();

    virtual std::string     GetShaderFile   () const override;

            void            Update          ( float t ) override;
    virtual void            Print           ( std::ostream &,int ) const {}

};

} // bv