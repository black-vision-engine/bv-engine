#include "DefaultGeometryPluginBase.h"

namespace bv { namespace model {

namespace DefaultSpring {

struct PN {
    static const std::string TESSELATION;
    static const std::string RADIUS;
    static const std::string RADIUSCROSSSECTION;
    static const std::string DELTA;
    static const std::string TURNS;
};

class PluginDesc : public DefaultGeometryPluginDescBase
{
public:
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;
    static  std::string                     UID                 ();
    PluginDesc();
    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
};

class Plugin : public DefaultGeometryPluginBase
{
public:
    ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_radius;
    ValueFloatPtr                               m_radiusCrossSection;
    ValueFloatPtr                               m_delta;
    ValueIntPtr                                 m_turns;

public:
    Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
private:
    virtual bool                                NeedsTopologyUpdate() override;
    virtual std::vector<IGeometryGeneratorPtr>  GetGenerators() override;
};

}

} }