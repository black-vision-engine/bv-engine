#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"

namespace bv { namespace model {

namespace DefaultCube {

class PluginDesc : public DefaultGeometryPluginDescBase
{
public:
    PluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

struct PN {
    static const std::string TESSELATION; // int tesselation
    static const std::string DIMENSIONS; // VecParam3 Size[x,y,z]
    static const std::string BEVEL; // VecParam1 Bevel 
    static const std::string WEIGHTCENTERX; // enum WeightCenter (MIN, MAX, CENTER)
	static const std::string WEIGHTCENTERY;
	static const std::string WEIGHTCENTERZ;
	static const std::string MAPPINGTYPE;
};

class Plugin : public DefaultGeometryPluginBase
{
public:
    enum WeightCenter : int { MAX, MIN, CENTER };
	enum MappingType : int { OLDSTYLE/*, SPHERICAL*/, GOODMAPPING };

    ValueIntPtr										    m_tesselation;
    ValueFloatPtr										m_bevel;
    ValueVec3Ptr										m_dimensions;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterX;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterY;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterZ;
	std::shared_ptr< ParamEnum< MappingType >	>		m_mappingType;
private:
    virtual std::vector<IGeometryGeneratorPtr>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
public:
    Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

}

DEFINE_ENUM_SET_PARAMETER( DefaultCube::Plugin::WeightCenter );
DEFINE_ENUM_SET_PARAMETER( DefaultCube::Plugin::MappingType );


} }