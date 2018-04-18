#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

namespace bv { namespace model {

namespace DefaultWallPlugin {


class PluginDesc : public DefaultGeometryPluginDescBase
{
public:
    PluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};



struct PN
{
    static const std::string HEIGHT;
    static const std::string WIDTH;
    static const std::string DEPTH;
    static const std::string TESSELATION;
    static const std::string ANGLE;
    static const std::string FRONT;
};



/**@brief 
@ingroup PluginsList*/
class DefaultPlugin : public DefaultGeometryPluginBase
{
public:

	ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_height,
                                                m_width,
                                                m_depth,
                                                m_angle;
    ValueBoolPtr                                m_front;

private:
    virtual std::vector<IGeometryGeneratorPtr>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
public:
    DefaultPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

}   //DefaultWallPlugin
}   // model
}   // bv