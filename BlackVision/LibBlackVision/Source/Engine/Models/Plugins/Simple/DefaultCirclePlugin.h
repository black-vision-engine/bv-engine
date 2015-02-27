#include "DefaultGeometryPluginBase.h"

namespace bv { namespace model {

class DefaultCirclePluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultCirclePluginDesc() 
        : DefaultGeometryPluginDescBase( UID(), "circle" ) { }

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

class DefaultCirclePlugin : public DefaultGeometryPluginBase
{
public:
    enum OpenAngleMode : int { CW, CCW, SYMMETRIC };

    static const std::string PN_TESSELATION;
    static const std::string PN_INNER_RADIUS;
    static const std::string PN_OUTER_RADIUS;
    static const std::string PN_OPEN_ANGLE;
    static const std::string PN_OPEN_ANGLE_MODE;

private:
    virtual IGeometryGenerator*                 GetGenerator() override;

    virtual bool                                NeedsTopologyUpdate();

    int                                         GetTesselation();
    float                                       GetInnerRadius();
    float                                       GetOuterRadius();
    float                                       GetOpenAngle();
    OpenAngleMode                               GetOpenAngleMode();
public:
    DefaultCirclePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
        : DefaultGeometryPluginBase( name, uid, prev, model ) { }

    //virtual void								Update                      ( TimeType t );
};

} }