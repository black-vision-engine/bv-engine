#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

namespace bv { namespace model {

namespace DefaultSpring {

struct PN {
    static const std::string TESSELATION;
	static const std::string TESSELATION2;
    static const std::string RADIUS;
    static const std::string RADIUSCROSSSECTION;
    static const std::string DELTA;
    static const std::string TURNS;
	static const std::string WEIGHTCENTERX;
	static const std::string WEIGHTCENTERY;
	static const std::string WEIGHTCENTERZ;
	static const std::string MAPPINGTYPE;
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
    enum WeightCenter : int{ MAX, MIN, CENTER };
	enum MappingType : int { OLDSTYLE/*, SPHERICAL*/, DOUBLETEXTURE };


    Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
private:
    virtual bool                                NeedsTopologyUpdate() override;
    virtual std::vector<IGeometryGeneratorPtr>  GetGenerators() override;
};

}


template<>
inline bool SetParameter< DefaultSpring::Plugin::WeightCenter >( IParameterPtr param, TimeType t, const DefaultSpring::Plugin::WeightCenter & val )
{
    //return SetSimpleTypedParameter< DefaultCone::DefaultConePlugin::WeightCenter> >( param, t, val );
    typedef ParamEnum<DefaultSpring::Plugin::WeightCenter> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

template<>
inline bool SetParameter< DefaultSpring::Plugin::MappingType >( IParameterPtr param, TimeType t, const DefaultSpring::Plugin::MappingType & val )
{
    //return SetSimpleTypedParameter< DefaultCone::DefaultConePlugin::WeightCenter> >( param, t, val );
    typedef ParamEnum<DefaultSpring::Plugin::MappingType> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

} }