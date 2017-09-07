#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

namespace bv {
namespace model
{


// ***********************
//
class DefaultEllipsePluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultEllipsePluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

class DefaultEllipsePlugin : public DefaultGeometryPluginBase
{
public:
    enum OpenAngleMode : int { CW, CCW, SYMMETRIC };

    static const std::string PN_QUALITY;
    static const std::string PN_OUTER_RADIUS1;
    static const std::string PN_OUTER_RADIUS2;
    static const std::string PN_OPEN_ANGLE;
    static const std::string PN_OPEN_ANGLE_MODE;

private:
    virtual std::vector<IGeometryGeneratorPtr>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate();

    float                                       GetQuality();
    float                                       GetOuterRadius1();
    float                                       GetOuterRadius2();
    float                                       GetOpenAngle();
    OpenAngleMode                               GetOpenAngleMode();
public:
    DefaultEllipsePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

DEFINE_ENUM_SET_PARAMETER( DefaultEllipsePlugin::OpenAngleMode );


} }