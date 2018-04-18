#pragma once


#pragma once


#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

namespace bv { namespace model { namespace DefaultCogWheel {


class PluginDesc : public DefaultGeometryPluginDescBase
{
public:
    PluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

struct PN
{
    static const std::string INNER_RADIUS;
	static const std::string OUTER_RADIUS;
	static const std::string TOOTH_HEIGHT;
	static const std::string TOOTH_BASE_LENGTH;
	static const std::string TOOTH_TOP_LENGTH;
	static const std::string TEETH_NUMBER;
	static const std::string BEVEL;
	static const std::string TESSELATION;
	static const std::string HEIGHT;
	static const std::string WEIGHTCENTERX; // enum WeightCenter (MIN, MAX, CENTER)
	static const std::string WEIGHTCENTERY;
	static const std::string WEIGHTCENTERZ;
};

/**@brief Plugin generating cogwheel.
@ingroup PluginsList*/
class Plugin : public DefaultGeometryPluginBase
{
public:
	enum WeightCenter : int { MAX, MIN, CENTER };

private:
	ValueFloatPtr		innerRadius;
	ValueFloatPtr		outerRadius;
	ValueFloatPtr		toothHeight;
	ValueFloatPtr		toothBaseLength;
	ValueFloatPtr		toothTopLength;
	ValueIntPtr			teethNumber;
	ValueFloatPtr		bevel;
	ValueIntPtr			tesselation;
	ValueFloatPtr		height;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterX;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterY;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterZ;
public:
	Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
	~Plugin();


	virtual std::vector<IGeometryGeneratorPtr> Plugin::GetGenerators() override;
	virtual bool Plugin::NeedsTopologyUpdate() override;
};

} // DefaultCogWheel


template<>
inline bool SetParameter< DefaultCogWheel::Plugin::WeightCenter >( IParameterPtr param, TimeType t, const DefaultCogWheel::Plugin::WeightCenter & val )
{
    //return SetSimpleTypedParameter< DefaultCone::DefaultConePlugin::WeightCenter> >( param, t, val );
    typedef ParamEnum<DefaultCogWheel::Plugin::WeightCenter> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}


} // model
} // bv

