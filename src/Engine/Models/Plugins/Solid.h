#pragma once 

#include "Engine/Models/Plugin.h"
#include "Engine/Models/Plugins/PluginParameters.h"

namespace bv {

class ParamVec4;
class ValueVec4;

// ***************************** DESCRIPTOR **********************************
class SolidParametersDescriptor : public BaseParametersDescriptor
{
public:

    static const std::string pluginName;

    static const std::string colorParamName;

    explicit SolidParametersDescriptor();
};


// ***************************** PLUGIN ********************************** 
class SolidColorPlugin : public BasePlugin< IShaderPlugin, SolidParametersDescriptor >
{
private:

    //FIXME: some utils should be added to keep these pairs as pairs (if there is a 1-1 mapping)
    ParamVec4 *                     m_colorParam;
    ValueVec4 *                     m_colorValue;

public:

    explicit            SolidColorPlugin    ( const Vec4Interpolator & col );
                        ~SolidColorPlugin   ();

    virtual std::string GetShaderFile       () const override;


    void                Update              ( float t ) override;
    void                Print               ( std::ostream & out, int tabs = 0 ) const override;

};

} // bv