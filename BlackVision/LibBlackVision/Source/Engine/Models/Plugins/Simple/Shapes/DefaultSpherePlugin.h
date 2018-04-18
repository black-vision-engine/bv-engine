#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv { namespace model { namespace DefaultSphere {


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
    static const std::string VERTICAL_STRIPES;
	static const std::string HORIZONTAL_STRIPES;
	static const std::string RADIUS;
	static const std::string OPEN_ANGLE;
	static const std::string OPEN_ANGLE_MODE;
	static const std::string MAPPING_TYPE;
};


/**@brief Sphere generator plugin.
@ingroup PluginsList*/
class Plugin : public DefaultGeometryPluginBase
{
public:
	enum OpenAngleMode : int { CW, CCW, SYMMETRIC };
	enum MappingType : int { SINGLETEXTURE, DOUBLETEXTURE };
private:
	ValueIntPtr		horizontal_stripes;
	ValueIntPtr		vertical_stripes;
	ValueFloatPtr	radius;
	ValueFloatPtr	open_angle;

	std::shared_ptr< ParamEnum< OpenAngleMode > >		open_angle_mode;
	std::shared_ptr< ParamEnum< MappingType > >			mapping_type;

public:
	Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
	~Plugin();

	virtual std::vector<IGeometryGeneratorPtr> Plugin::GetGenerators() override;
	virtual bool Plugin::NeedsTopologyUpdate() override;
};



}	//DefaultSphere


DEFINE_ENUM_SET_PARAMETER( DefaultSphere::Plugin::OpenAngleMode );
DEFINE_ENUM_SET_PARAMETER( DefaultSphere::Plugin::MappingType );


}}