#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

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
    static const std::string TESSELATION;
	static const std::string RADIUS;
};

/**@brief Plugin generates geosphere built from tetrahedron.

Tessellation parameter discribes number of divisions of face of the tetrahedron.
Each face will be subdivided into 4 triangles in single tesselletion step.

@note UV mapping isn't made as usual for simple sphere. It isn't easy to describe.
Better add a texture to geosphere and check it for yourself.

@ingroup PluginsList*/
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