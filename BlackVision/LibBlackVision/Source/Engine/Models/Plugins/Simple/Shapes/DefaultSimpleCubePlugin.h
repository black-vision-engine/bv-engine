#pragma once


#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

namespace bv { namespace model { namespace DefaultSimpleCube {


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
    static const std::string DIMENSIONS; // VecParam3 Size[x,y,z]
};

/**@brief Plugin generating cube built as two triangle strips.

UVs cooridiantes are made as follows:
- top triangle strip will be mapped to rectangle (0.0, 0.0) (0.0, 0.5) (1.0, 0.0) (1.0, 0.5)
- bottom triangle strip will be mapped to rectangle (0.0, 0.5) (1.0, 0.5) (1.0, 1.0) (0.0, 1.0)

@ingroup PluginsList*/
class Plugin : public DefaultGeometryPluginBase
{
private:
	ValueVec3Ptr		dimensions;

public:
	Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
	~Plugin();


	virtual std::vector<IGeometryGeneratorPtr> Plugin::GetGenerators() override;
	virtual bool Plugin::NeedsTopologyUpdate() override;
};




} // DefaultSimpleCube
} // model
} // bv