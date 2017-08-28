#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv {
namespace model
{

namespace DefaultTorus {



// ***********************
//
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
    static const std::string WEIGHTCENTERX; // enum WeightCenter (MIN, MAX, CENTER)
	static const std::string WEIGHTCENTERY;
	static const std::string WEIGHTCENTERZ;
	static const std::string MAPPINGTYPE;
};

class Plugin : public DefaultGeometryPluginBase
{
public:
	enum OpenAngleMode : int { CW, CCW, SYMMETRIC };
	enum WeightCenter : int { MAX, MIN, CENTER };
	enum MappingType : int { OLDSTYLE/*, SPHERICAL*/, DOUBLETEXTURE };


    ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_radius;
    ValueFloatPtr                               m_radiusCrossSection;
    ValueFloatPtr                               m_openAngle;
	std::shared_ptr< ParamEnum< OpenAngleMode > >		m_openAngleMode;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterX;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterY;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterZ;
	std::shared_ptr< ParamEnum< MappingType >	>		m_mappingType;

private:
    virtual std::vector<IGeometryGeneratorPtr>  GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
public:
    Plugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};


}

DEFINE_ENUM_SET_PARAMETER( DefaultTorus::Plugin::OpenAngleMode );
DEFINE_ENUM_SET_PARAMETER( DefaultTorus::Plugin::WeightCenter );
DEFINE_ENUM_SET_PARAMETER( DefaultTorus::Plugin::MappingType );


} }