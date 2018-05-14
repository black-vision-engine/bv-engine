#pragma once

#include "Engine/Models/Plugins/Simple/DefaultGeometryPluginBase.h"
#include "Engine/Models/Plugins/Descriptor/ModelHelper.h"



namespace bv {
namespace model
{

namespace DefaultCone {


// ***********************
//
class DefaultConePluginDesc : public DefaultGeometryPluginDescBase
{
public:
    DefaultConePluginDesc();

    virtual DefaultPluginParamValModelPtr   CreateDefaultModel  ( ITimeEvaluatorPtr timeEvaluator ) const override;
    virtual IPluginPtr                      CreatePlugin        ( const std::string & name, IPluginPtr prev, ITimeEvaluatorPtr timeEvaluator ) const override;

    static  std::string                     UID                 ();
};

struct PN {
    static const std::string TESSELATION; // int tesselation
    static const std::string HEIGHT; // VecParam1 Height
    static const std::string INNERRADIUS; // VecParam1 InnerRadius
    static const std::string OUTERRADIUS; // VecParam1 OuterRadius
    static const std::string INNERHEIGHT; // VecParam1 InnerHeight
    static const std::string ROUNDEDTIPHEIGHT; // VecParam1 RoundedTipHeight (Bevel)
    static const std::string OPENANGLE; // VecParam1 OpenAngle
    static const std::string WEIGHTCENTERX; // enum WeightCenter (MIN, MAX, CENTER)
    static const std::string WEIGHTCENTERY;
    static const std::string WEIGHTCENTERZ;
    static const std::string OPENANGLEMODE; // enum OpenAngleMode(Clockwise, CCW, symetric)
    static const std::string BEVELTESSELATION;
    static const std::string MAPPINGTYPE;
};


/**@brief Plugins generates rounded cone.
@ingroup PluginsList*/
class DefaultConePlugin : public DefaultGeometryPluginBase
{
public:
    enum OpenAngleMode : int { CW, CCW, SYMMETRIC };
    enum WeightCenter : int { MAX, MIN, CENTER };
    enum MappingType : int { OLDSTYLE/*, SPHERICAL*/, GOODMAPPING };

    ValueIntPtr                                 m_tesselation, m_bevelTesselation;
    ValueFloatPtr                               m_innerRadius,
                                                m_outerRadius,
                                                m_innerHeight,
                                                m_roundedTipHeight,
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
    DefaultConePlugin( const std::string & name, const std::string & uid, IPluginPtr prev, IPluginParamValModelPtr model );
};

}

DEFINE_ENUM_SET_PARAMETER( DefaultCone::DefaultConePlugin::OpenAngleMode );
DEFINE_ENUM_SET_PARAMETER( DefaultCone::DefaultConePlugin::WeightCenter );
DEFINE_ENUM_SET_PARAMETER( DefaultCone::DefaultConePlugin::MappingType );

} }


