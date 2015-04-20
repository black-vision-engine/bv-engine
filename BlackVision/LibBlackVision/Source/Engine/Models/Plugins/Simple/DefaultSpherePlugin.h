#pragma once

#include "defaultgeometrypluginbase.h"


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
};



class Plugin : public DefaultGeometryPluginBase
{
private:
	ValueIntPtr		horizontal_stripes;
	ValueIntPtr		vertical_stripes;
	ValueFloatPtr	radius;
public:
	Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
	~Plugin();

	virtual std::vector<IGeometryGeneratorPtr> Plugin::GetGenerators() override;
	virtual bool Plugin::NeedsTopologyUpdate() override;
};




}}}