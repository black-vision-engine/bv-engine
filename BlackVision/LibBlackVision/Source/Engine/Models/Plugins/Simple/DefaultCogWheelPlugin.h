#pragma once


#pragma once


#include "defaultgeometrypluginbase.h"

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
};

/**@brief Plugin containing cube built as two triangle strips.

UVs cooridiantes are made as follows:
- top triangle strip will be mapped to rectangle (0.0, 0.0) (0.0, 0.5) (1.0, 0.0) (1.0, 0.5)
- bottom triangle strip will be mapped to rectangle (0.0, 0.5) (1.0, 0.5) (1.0, 1.0) (0.0, 1.0)
*/
class Plugin : public DefaultGeometryPluginBase
{
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

public:
	Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
	~Plugin();


	virtual std::vector<IGeometryGeneratorPtr> Plugin::GetGenerators() override;
	virtual bool Plugin::NeedsTopologyUpdate() override;
};




} // DefaultSimpleCube
} // model
} // bv

