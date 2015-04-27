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
	enum OpenAngleMode : int { CW, CCW, SYMMETRIC };

    ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_radius;
    ValueFloatPtr                               m_radiusCrossSection;
    ValueFloatPtr                               m_openAngle;
	std::shared_ptr< ParamEnum< OpenAngleMode > >		m_openAngleMode;

private:
    virtual std::vector<IGeometryGeneratorPtr>  GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
public:
    Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};


}

// Nie patrzeæ w dó³!!! Brzydkie !!!!!!

template<>
inline bool SetParameter< DefaultTorus::Plugin::OpenAngleMode >( IParameterPtr param, TimeType t, const DefaultTorus::Plugin::OpenAngleMode & val )
{
    //return SetSimpleTypedParameter< ParamEnum<DefaultCirclePlugin::OpenAngleMode> >( param, t, val );
    typedef ParamEnum<DefaultTorus::Plugin::OpenAngleMode> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

} }