#include "DefaultGeometryPluginBase.h"

namespace bv { namespace model {

namespace DefaultCube {

class PluginDesc : public DefaultGeometryPluginDescBase
{
public:
    PluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

struct PN {
    static const std::string TESSELATION; // int tesselation
    static const std::string DIMENSIONS; // VecParam3 Size[x,y,z]
    static const std::string BEVEL; // VecParam1 Bevel 
};

class Plugin : public DefaultGeometryPluginBase
{
public:
    ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_bevel;
    ValueVec3Ptr                                m_dimensions;

private:
    virtual std::vector<IGeometryGeneratorPtr>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
public:
    Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

}

} }