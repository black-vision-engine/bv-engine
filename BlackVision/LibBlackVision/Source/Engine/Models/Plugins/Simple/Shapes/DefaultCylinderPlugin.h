#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"


namespace bv { namespace model {

namespace DefaultCylinder {


class DefaultCylinderPluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultCylinderPluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};



struct PN {
    static const std::string TESSELATION; // int tesselation
    static const std::string HEIGHT; // VecParam1 Height
    static const std::string INNERRADIUS; // VecParam1 InnerRadius
    static const std::string OUTERRADIUS; // VecParam1 OuterRadius
    static const std::string OPENANGLE; // VecParam1 OpenAngle
    static const std::string OPENANGLEMODE; // enum OpenAngleMode(Clockwise, CCW, symetric)
	static const std::string WEIGHTCENTERX;
	static const std::string WEIGHTCENTERY;
	static const std::string WEIGHTCENTERZ;
	static const std::string MAPPINGTYPE;
};



/**@brief Plugins generate cylinder.
@ingroup PluginsList*/
class DefaultPlugin : public DefaultGeometryPluginBase
{
public:
	enum OpenAngleMode : int { CW, CCW, SYMMETRIC };
	enum WeightCenter : int { MIN, MAX, CENTER };
	enum MappingType : int { OLDSTYLE/*, SPHERICAL*/, GOODMAPPING };

	ValueIntPtr                                 m_tesselation;
    ValueFloatPtr                               m_innerRadius,
                                                m_outerRadius,
                                                m_openAngle,
                                                m_height;
	std::shared_ptr< ParamEnum< OpenAngleMode > >		m_openAngleMode;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterX;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterY;
	std::shared_ptr< ParamEnum< WeightCenter >	>		m_weightCenterZ;
	std::shared_ptr< ParamEnum< MappingType >	>		m_mappingType;

private:
    virtual std::vector<IGeometryGeneratorPtr>    GetGenerators() override;

    virtual bool                                NeedsTopologyUpdate() override;
public:
    DefaultPlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

} //DefaultCylinder


DEFINE_ENUM_SET_PARAMETER( DefaultCylinder::DefaultPlugin::OpenAngleMode );
DEFINE_ENUM_SET_PARAMETER( DefaultCylinder::DefaultPlugin::WeightCenter );
DEFINE_ENUM_SET_PARAMETER( DefaultCylinder::DefaultPlugin::MappingType );



} // model
} // bv
