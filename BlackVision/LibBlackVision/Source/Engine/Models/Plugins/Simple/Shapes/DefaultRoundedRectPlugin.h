#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

namespace bv { namespace model {

class DefaultRoundedRectPluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultRoundedRectPluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

class DefaultRoundedRectPlugin : public DefaultGeometryPluginBase
{
public:
    static const std::string PN_SIZE;
    static const std::string PN_BEVELS;
    static const std::string PN_TESSELATION;

private:
    virtual std::vector<IGeometryGeneratorPtr>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate();

    glm::vec2                                   GetSize();
    glm::vec4                                   GetBevels();
    int                                         GetTesselation();
public:
    DefaultRoundedRectPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

} }