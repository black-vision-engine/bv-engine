#pragma once

#include "Engine/Models/Plugins/PluginParameters.h"
#include "Engine/Models/Plugin.h"

#include "Mathematics/Transform/MatTransform.h"
#include "Engine/Models/Parameter.h"

namespace bv {

class BuildScene;

class WypierdalajParamDescriptor : public BaseParametersDescriptor 
{
public:

    static const std::string  pluginName;

    explicit WypierdalajParamDescriptor()
        : BaseParametersDescriptor( pluginName )
    {
    }
};

class Transformation : public BasePlugin<ITransformPlugin, WypierdalajParamDescriptor>
{
public:
    Transformation(const std::string& name, const TransformF& trans)
       // : BasePlugin("Transformation")
    {
        if(name == "MVP")
        {
            //addParameter(new ParamTransform(name, trans, ParameterSemantic::LOCAL_TRANSFORM));
        }
        else
        {
            //addParameter(new ParamTransform(name, trans));
        }
        
    }

    virtual const glm::mat4& TransformMatrix() const
    {
        throw;
    }

    void    Update(float t) override
    {}

    void    Print(std::ostream& out, int tabs = 0) const override
    {}

    void    Dummy();
};




} // bv
