#include "DefaultGeometryPluginBase.h"

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

private:
    virtual std::vector<IGeometryGenerator*>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate();

    glm::vec2                                   GetSize();
    glm::vec4                                   GetBevels();
public:
    DefaultRoundedRectPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model ) 
        : DefaultGeometryPluginBase( name, uid, prev, model ) { }
};

} }