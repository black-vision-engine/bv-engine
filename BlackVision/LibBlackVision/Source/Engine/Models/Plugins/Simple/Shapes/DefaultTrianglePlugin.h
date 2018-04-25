#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

namespace bv { namespace model {


// ***********************
//
class DefaultTrianglePluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultTrianglePluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};



/**@brief 
@ingroup PluginsList*/
class DefaultTrianglePlugin : public DefaultGeometryPluginBase
{
public:
    static const std::string PN_POINTA;
    static const std::string PN_POINTB;
    static const std::string PN_POINTC;

private:
    virtual std::vector<IGeometryGeneratorPtr>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate();

    glm::vec3                                   GetPointA();
    glm::vec3                                   GetPointB();
    glm::vec3                                   GetPointC();
public:
    DefaultTrianglePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

} }