#pragma once

#include "defaultgeometrypluginbase.h"

namespace bv { namespace model { namespace DefaultGeosphere {


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
    static const std::string TESSELLATION;
	static const std::string RADIUS;
};


class Plugin : public DefaultGeometryPluginBase
{
private:
	ValueIntPtr		tesselletion;
	ValueFloatPtr	radius;
public:
	Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
	~Plugin(void);

	virtual std::vector<IGeometryGeneratorPtr> Plugin::GetGenerators() override;	///<Zwraca tablicê generatorów geometrii.
	virtual bool Plugin::NeedsTopologyUpdate() override;							///<Zwraca true je¿eli jakiœ parametr siê zmieni³ i trzeba przebudowaæ geometriê.
};



}}}