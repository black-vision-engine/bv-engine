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
    static const std::string TESSELATION;       // int tesselation
    static const std::string DIMENSIONS;        // VecParam3 Size[x,y,z]
    static const std::string BEVEL;             // VecParam1 Bevel 
    static const std::string WEIGHTCENTERX;     // enum WeightCenter (MIN, MAX, CENTER)
	static const std::string WEIGHTCENTERY;
	static const std::string WEIGHTCENTERZ;
	static const std::string MAPPINGTYPE;
    static const std::string SMOOTH_BEVEL;
};

class Plugin : public DefaultGeometryPluginBase
{
public:
    enum WeightCenter : int { MAX, MIN, CENTER };
	enum MappingType : int { OLDSTYLE/*, SPHERICAL*/, GOODMAPPING };

public:

    ValueBoolPtr                                        m_smooth;
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


template<>
inline bool SetParameter< DefaultCube::Plugin::WeightCenter >( IParameterPtr param, TimeType t, const DefaultCube::Plugin::WeightCenter & val )
{
    //return SetSimpleTypedParameter< DefaultCone::DefaultConePlugin::WeightCenter> >( param, t, val );
    typedef ParamEnum<DefaultCube::Plugin::WeightCenter> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

template<>
inline bool SetParameter< DefaultCube::Plugin::MappingType >( IParameterPtr param, TimeType t, const DefaultCube::Plugin::MappingType & val )
{
    //return SetSimpleTypedParameter< DefaultCone::DefaultConePlugin::WeightCenter> >( param, t, val );
    typedef ParamEnum<DefaultCube::Plugin::MappingType> ParamType;

    ParamType * typedParam = QueryTypedParam< std::shared_ptr< ParamType > >( param ).get();

    if( typedParam == nullptr )
    {
        return false;
    }

    typedParam->SetVal( val, t );

    return true;
}

} }