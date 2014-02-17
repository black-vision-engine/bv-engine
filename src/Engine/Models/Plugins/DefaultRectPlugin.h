#pragma once

#include "Engine/Models/Plugins/Plugin.h"
#include "Engine/Models/Plugins/ParamValModel/DefaultPluginParamValModel.h"


namespace bv { namespace model {

class DefaultRectPlugin;
class RectComponent;

// ***************************** DESC **********************************
class DefaultRectPluginDesc
{
public:

    static const char *                 GetName()   { return "default_rect_plugin"; }
    static DefaultPluginParamValModel * CreateModel ( bool setDefaultValues );
    static DefaultRectPlugin *          CreatePlugin( const IPlugin * prev, bool setDefaultValues = true );

};


// ***************************** PLUGIN ********************************** 
class DefaultRectPlugin : public BasePlugin< IPlugin, DefaultRectPluginDesc >
{
private:

    DefaultPluginParamValModelPtr       m_paramValModel;
    model::VertexAttributesChannel *    m_vaChannel;

    ParamFloat *                        m_widthParam;
    ParamFloat *                        m_heightParam;

    float                               m_lastW;
    float                               m_lastH;

    RectComponent *                     m_rct;

private:

    explicit            DefaultRectPlugin    ( const IPlugin * prev, DefaultPluginParamValModelPtr model );

public:

                        ~DefaultRectPlugin   ();

    virtual const IVertexAttributesChannel *    GetVertexAttributesChannel  () const override;

    virtual void                                Update                      ( TimeType t ) override;

    friend class DefaultRectPluginDesc;

};

} // model
} // bv
