#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"


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

template<>
inline bool SetParameter< DefaultSphere::Plugin::OpenAngleMode >( IParameterPtr param, TimeType t, const DefaultSphere::Plugin::OpenAngleMode & val )
{
    //return SetSimpleTypedParameter< ParamEnum<DefaultCirclePlugin::OpenAngleMode> >( param, t, val );
    typedef ParamEnum<DefaultSphere::Plugin::OpenAngleMode> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}	

template<>
inline bool SetParameter< DefaultSphere::Plugin::MappingType >( IParameterPtr param, TimeType t, const DefaultSphere::Plugin::MappingType & val )
{
    //return SetSimpleTypedParameter< ParamEnum<DefaultCirclePlugin::OpenAngleMode> >( param, t, val );
    typedef ParamEnum<DefaultSphere::Plugin::MappingType> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}	

}}