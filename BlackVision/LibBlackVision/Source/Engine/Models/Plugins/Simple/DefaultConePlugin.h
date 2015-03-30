#include "DefaultGeometryPluginBase.h"

namespace bv { namespace model {

namespace DefaultCone {

class DefaultConePluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultConePluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

struct PN {
    static const std::string TESSELATION; // int tesselation
    static const std::string HEIGHT; // VecParam1 Height
    static const std::string INNERRADIUS; // VecParam1 InnerRadius
    static const std::string OUTERRADIUS; // VecParam1 OuterRadius
    static const std::string INNERHEIGHT; // VecParam1 InnerHeight
    static const std::string ROUNDEDTIPHEIGHT; // VecParam1 RoundedTipHeight (Bevel)
    static const std::string OPENANGLE; // VecParam1 OpenAngle
    static const std::string WEIGHTCENTER; // enum WeightCenter (TOP, BOTTOM, CENTER)
    static const std::string OPENANGLEMODE; // enum OpenAngleMode(Clockwise, CCW, symetric)
};

class DefaultConePlugin : public DefaultGeometryPluginBase
{
public:
    enum OpenAngleMode : int { CW, CCW, SYMMETRIC };
    enum WeightCenter : int { TOP, BOTTOM, CENTER };

    ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_innerRadius,
                                                m_outerRadius,
                                                m_innerHeight,
                                                m_roundedTipHeight,
                                                m_openAngle;

private:
    virtual IGeometryGenerator*                 GetGenerator() override;

    virtual bool                                NeedsTopologyUpdate() override;
public:
    DefaultConePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

}

} }