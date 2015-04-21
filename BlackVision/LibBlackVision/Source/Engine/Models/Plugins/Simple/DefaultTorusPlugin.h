#include "DefaultGeometryPluginBase.h"

namespace bv { namespace model {

namespace DefaultTorus {

class PluginDesc : public DefaultGeometryPluginDescBase
{
public:
    PluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

struct PN {
    static const std::string TESSELATION;
    static const std::string RADIUS;
    static const std::string RADIUSCROSSSECTION;
    static const std::string OPENANGLE;
    static const std::string OPENANGLEMODE;
};

class Plugin : public DefaultGeometryPluginBase
{
public:
    ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_radius;
    ValueFloatPtr                               m_radiusCrossSection;
    ValueFloatPtr                               m_openAngle;

private:
    virtual std::vector<IGeometryGeneratorPtr>  GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
public:
    Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

}

} }